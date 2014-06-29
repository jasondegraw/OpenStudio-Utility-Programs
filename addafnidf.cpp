/**********************************************************************
 *  Copyright (c) 2014 Jason W. DeGraw
 *  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 **********************************************************************/

#include <model/Model.hpp>
#include <osversion/VersionTranslator.hpp>
#include <utilities/core/CommandLine.hpp>
#include <utilities/core/Path.hpp>
#include <utilities/idf/IdfObject.hpp>
#include <model/ThermalZone.hpp>
#include <model/Surface.hpp>
#include <model/SubSurface.hpp>
#include <model/AirflowNetworkSimulationControl_Impl.hpp>
#include <energyplus/ForwardTranslator.hpp>

#include <string>
#include <iostream>

using namespace openstudio;
using namespace openstudio::model;

class AirflowNetworkBuilder : public openstudio::model::detail::SurfaceNetworkBuilder
{
public:
  explicit AirflowNetworkBuilder(bool linkSubSurfaces=false);

  std::vector<IdfObject> idfObjects();

protected:
  virtual bool linkExteriorSurface(const ThermalZone &zone, const Space &space, const Surface &surface);
  virtual bool linkExteriorSubSurface(const ThermalZone &zone, const Space &space, const Surface &surface, const SubSurface &subSurface);
  virtual bool linkInteriorSurface(const ThermalZone &zone, const Space &space, const Surface &surface, 
    const Surface &adjacentSurface, const Space &adjacentSpace, const ThermalZone &adjacentZone);
  virtual bool linkInteriorSubSurface(const ThermalZone &zone, const Space &space, const Surface &surface, const SubSurface &subSurface,
    const SubSurface &adjacentSubSurface, const Surface &adjacentSurface, const Space &adjacentSpace, const ThermalZone &adjacentZone);

private:
  bool linkSurface(const std::string &elementName, const Surface &surface, std::vector<IdfObject> &surfaces);
  std::vector<IdfObject> m_idfObjects;
  std::vector<IdfObject> m_airflowElements;
  std::vector<IdfObject> m_interiorSurfaces;
  std::vector<IdfObject> m_exteriorSurfaces;
  bool m_includeSubSurfaces;
  std::map<std::string,double> m_maxArea;

  REGISTER_LOGGER("openstudio.model.detail.AirflowNetworkBuilder");
};

AirflowNetworkBuilder::AirflowNetworkBuilder(bool includeSubSurfaces) : SurfaceNetworkBuilder(nullptr),m_includeSubSurfaces(includeSubSurfaces)
{
}

std::vector<IdfObject> AirflowNetworkBuilder::idfObjects()
{
  std::cout << "Maximum exterior area: " << m_maxArea["Exterior"] << std::endl;
  std::cout << "Maximum interior area: " << m_maxArea["Interior"] << std::endl;

  double maxArea = m_maxArea["Exterior"];
  if(m_maxArea["Interior"] > maxArea) {
    maxArea = m_maxArea["Interior"];
  }

  QStringList idfStrings;

  idfStrings.clear();
  idfStrings << "AirflowNetwork:MultiZone:ReferenceCrackConditions"
    << "ReferenceCrackConditions"  // !- Name of Reference Crack Conditions
    << "20.0"  // !- Reference Temperature for Crack Data {C}
    << "101325"  // !- Reference Barometric Pressure for Crack Data {Pa}
    << "0.0"; // !- Reference Humidity Ratio for Crack Data {kgWater/kgDryAir}
  m_airflowElements.push_back(openstudio::IdfObject::load((idfStrings.join(",")+";").toStdString()).get());

  // This is the "two elements to rule them all" approach
  // Generate exterior leakage element
  idfStrings.clear();
  idfStrings << "AirflowNetwork:MultiZone:Surface:Crack"
    << "ExteriorComponent"  // !- Name of Surface Crack Component
    << QString().sprintf("%g",m_maxArea["Exterior"]*4.99082e-4) // !- Air Mass Flow Coefficient at Reference Conditions {kg/s}
    << "0.65"  // !- Air Mass Flow Exponent {dimensionless}
    << "ReferenceCrackConditions"; // !- Reference Crack Conditions
  m_airflowElements.push_back(openstudio::IdfObject::load((idfStrings.join(",")+";").toStdString()).get());

  // Generate interior leakage element
  idfStrings.clear();
  idfStrings << "AirflowNetwork:MultiZone:Surface:Crack"
    << "InteriorComponent"  // !- Name of Surface Crack Component
    << QString().sprintf("%g",m_maxArea["Interior"]*2.0*4.99082e-4) // !- Air Mass Flow Coefficient at Reference Conditions {kg/s}
    << "0.65"  // !- Air Mass Flow Exponent {dimensionless}
    << "ReferenceCrackConditions"; // !- Reference Crack Conditions
  m_airflowElements.push_back(openstudio::IdfObject::load((idfStrings.join(",")+";").toStdString()).get());

  // Set the multipliers on all the elements appropriately
  BOOST_FOREACH(IdfObject obj, m_exteriorSurfaces) {
  
  }

  BOOST_FOREACH(IdfObject obj, m_interiorSurfaces) {
  
  }

  std::vector<IdfObject> objects = m_airflowElements;
  objects.insert(objects.end(), m_exteriorSurfaces.begin(), m_exteriorSurfaces.end());
  objects.insert(objects.end(), m_interiorSurfaces.begin(), m_interiorSurfaces.end());

  return objects;
}

bool AirflowNetworkBuilder::linkSurface(const std::string &elementName, const Surface &surface, std::vector<IdfObject> &surfaces)
{
  QString idfFormat = QString("AirflowNetwork:MultiZone:Surface,%1,") + QString::fromStdString(elementName) + QString("%2,%3;");
  boost::optional<std::string> name = surface.name();
  if(!name) {
    LOG(Warn, "Surface '" << openstudio::toString(surface.handle()) << "' has no name, will not be present in airflow network.");
    return false;
  }

  // Get the surface area, store the maximum
  double surfaceArea = surface.grossArea();
  if(m_includeSubSurfaces) {
    surfaceArea = surface.netArea();
  }
  std::map<std::string,double>::iterator it = m_maxArea.find(elementName);
  if(it == m_maxArea.end()) {
    m_maxArea[elementName] = surfaceArea;
  } else {
    if(it->second < surfaceArea) {
      it->second = surfaceArea;
    }
  }
  
  // This is probably a bad way to store the surface area, should directly create the IdfObject
  QString idfString = idfFormat.arg(openstudio::toQString(*name)).arg("").arg(surfaceArea);
  boost::optional<IdfObject> obj = openstudio::IdfObject::load(idfString.toStdString());
  if(!obj) {
    LOG(Error, "Failed to generate AirflowNetwork surface for " << name.get());
    return false;
  }
  surfaces.push_back(obj.get());
  return true;
}

bool AirflowNetworkBuilder::linkExteriorSurface(const ThermalZone &zone, const Space &space, const Surface &surface)
{
  return linkSurface("Exterior", surface, m_exteriorSurfaces);
}

bool AirflowNetworkBuilder::linkInteriorSurface(const ThermalZone &zone, const Space &space, const Surface &surface, 
  const Surface &adjacentSurface, const Space &adjacentSpace, const ThermalZone &adjacentZone)
{
  return linkSurface("Interior", surface, m_interiorSurfaces);
}

bool AirflowNetworkBuilder::linkExteriorSubSurface(const ThermalZone &zone, const Space &space, const Surface &surface, const SubSurface &subSurface)
{
  return true;
}

bool AirflowNetworkBuilder::linkInteriorSubSurface(const ThermalZone &zone, const Space &space, const Surface &surface, const SubSurface &subSurface,
  const SubSurface &adjacentSubSurface, const Surface &adjacentSurface, const Space &adjacentSpace, const ThermalZone &adjacentZone)
{
  return true;
}

void usage( boost::program_options::options_description desc)
{
  std::cout << "Usage: addafnidf --inputPath=./path/to/input.osm" << std::endl;
  std::cout << "   or: addafnidf input.osm" << std::endl;
  std::cout << desc << std::endl;
}

int main(int argc, char *argv[])
{
  std::string inputPathString;

  boost::program_options::options_description desc("Allowed options");
  desc.add_options()
    ("help", "print help message")
    ("inputPath", boost::program_options::value<std::string>(&inputPathString), "path to OSM file");
  boost::program_options::positional_options_description pos;
  pos.add("inputPath", -1);

  boost::program_options::variables_map vm;
  // The following try/catch block is necessary to avoid uncaught
  // exceptions when the program is executed with more than one
  // "positional" argument - there's got to be a better way.
  try {
    boost::program_options::store(boost::program_options::command_line_parser(argc, argv).options(desc).positional(pos).run(),
      vm);
    boost::program_options::notify(vm);
  }
  catch(std::exception&) {
    std::cerr << "Execution failed: check arguments and retry."<< std::endl << std::endl;
    usage(desc);
    return EXIT_FAILURE;
  }
  if (vm.count("help")) {
    usage(desc);
    return EXIT_SUCCESS;
  }
  if(!vm.count("inputPath")) {
    std::cerr << "No input path given." << std::endl << std::endl;
    usage(desc);
    return EXIT_FAILURE;
  }

  openstudio::path inputPath = openstudio::toPath(inputPathString);

  if(!boost::filesystem::exists(inputPath)) {
    std::cerr << "Input path does not exist." << std::endl;
    return EXIT_FAILURE;
  }

  openstudio::osversion::VersionTranslator vt;
  boost::optional<openstudio::model::Model> model = vt.loadModel(inputPath);

  if(!model) {
    std::cerr << "Unable to load file '"<< inputPathString << "' as an OpenStudio model." << std::endl;
    return EXIT_FAILURE;
  }

  // Get an E+ workspace
  openstudio::energyplus::ForwardTranslator translator;
  openstudio::Workspace workspace = translator.translateModel(model.get(),nullptr);

  // Add AFN objects
  AirflowNetworkBuilder builder;
  builder.build(model.get());
  std::vector<openstudio::IdfObject> idfObjects = builder.idfObjects();
  if(!idfObjects.size()) {
    std::cerr << "No AirflowNetwork objects were added to model, no output IDF written." << std::endl;
    return EXIT_FAILURE;
  }

  std::cout << "Adding " << idfObjects.size() << " IDF objects to model." << std::endl;
  
  /*
  openstudio::path outPath = inputPath.replace_extension(openstudio::toPath("idf").string());

  if(!workspace.save(outPath,true)) {
    std::cerr << "Failed to write IDF file." << std::endl;
    return EXIT_FAILURE;
  }*/
  
  return EXIT_SUCCESS;
}

