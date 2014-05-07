/**********************************************************************
 *  Copyright (c) 2014, Jason W. DeGraw
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
#include <utilities/sql/SqlFile.hpp>
#include <utilities/filetypes/EpwFile.hpp>

#include <string>
#include <iostream>
#include <QFile>

void usage( boost::program_options::options_description desc)
{
  std::cout << "Usage: epwtest --input-path=./path/to/input.txt" << std::endl;
  std::cout << "   or: epwtest input.txt" << std::endl;
  std::cout << desc << std::endl;
}

int main(int argc, char *argv[])
{
  std::string inputPathString;
  std::string outputPathString;
  boost::program_options::options_description desc("Allowed options");

  desc.add_options()
    ("help,h", "print help message")
    ("input-path,i", boost::program_options::value<std::string>(&inputPathString), "path to input txt file")
    ("output-path,o", boost::program_options::value<std::string>(&outputPathString), "path to output csv file")
    ("quiet,q", "suppress progress output");

  boost::program_options::positional_options_description pos;
  pos.add("input-path", -1);
    
  boost::program_options::variables_map vm;
  // The following try/catch block is necessary to avoid uncaught
  // exceptions when the program is executed with more than one
  // "positional" argument - there's got to be a better way.
  try {
    boost::program_options::store(boost::program_options::command_line_parser(argc,
      argv).options(desc).positional(pos).run(), vm);
    boost::program_options::notify(vm);
  } catch(std::exception&) {
    std::cout << "Execution failed: check arguments and retry."<< std::endl << std::endl;
    usage(desc);
    return EXIT_FAILURE;
  }
  
  if(vm.count("help")) {
    usage(desc);
    return EXIT_SUCCESS;
  }

  if(!vm.count("input-path")) {
    std::cout << "No input path given." << std::endl << std::endl;
    usage(desc);
    return EXIT_FAILURE;
  }

  // Open the input text file, expect one EPW file per line
  QFile file(QString().fromStdString(inputPathString));
  
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    std::cout << "Failed to open input file '" << inputPathString << "'" << std::endl;
    return EXIT_FAILURE;
  }

  QTextStream in(&file);
  QString line = in.readLine();
  while (!line.isNull()) {
    openstudio::path inputPath = openstudio::toPath(line);
    boost::optional<openstudio::EpwFile> epwFile;
    epwFile = openstudio::EpwFile(inputPath,true);
    if(!epwFile) {
      std::cout << "Failed to read " << line.toStdString() <<
    }
    line = in.readLine();
  }
  return EXIT_SUCCESS;
}
