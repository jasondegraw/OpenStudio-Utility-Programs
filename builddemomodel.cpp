/**********************************************************************
 *  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
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
//#include <utilities/idf/IdfObject.hpp>
//#include <model/ThermalZone.hpp>
//#include <model/ThermalZone_Impl.hpp>
//#include <model/Surface.hpp>
//#include <model/SubSurface.hpp>

// Includes for Schedules
#include <model/ScheduleTypeLimits.hpp>
#include <model/DefaultScheduleSet.hpp>
#include <model/ScheduleRuleset.hpp>
#include <model/ScheduleDay.hpp>
#include <utilities/time/Time.hpp>
#include <model/ScheduleRule.hpp>

// Includes for Constructions
#include <model/DefaultConstructionSet.hpp>
#include <model/DefaultSurfaceConstructions.hpp>
#include <model/DefaultSubSurfaceConstructions.hpp>
#include <model/OpaqueMaterial.hpp>
#include <model/StandardOpaqueMaterial.hpp>
#include <model/AirGap.hpp>
#include <model/Construction.hpp>
#include <model/AirWallMaterial.hpp>
#include <model/MasslessOpaqueMaterial.hpp>
#include <model/FenestrationMaterial.hpp>
#include <model/SimpleGlazing.hpp>
#include <model/StandardGlazing.hpp>
#include <model/Gas.hpp>

//#include <model/AirflowNetworkSimulationControl_Impl.hpp>
//#include <energyplus/ForwardTranslator.hpp>
//#include <utilities/idd/AirflowNetwork_SimulationControl_FieldEnums.hxx>
//#include <utilities/idd/AirflowNetwork_MultiZone_Surface_FieldEnums.hxx>
//#include <utilities/idd/IddEnums.hxx>

#include <string>
#include <iostream>

using namespace openstudio;
using namespace openstudio::model;

void addExampleSchedules(Model& model) {

  // nice to pre-define Temperature limits because default limits do not provide bounds
  ScheduleTypeLimits temperatureLimits(model);
  temperatureLimits.setName("Temperature");
  temperatureLimits.setLowerLimitValue(-60.0);
  temperatureLimits.setUpperLimitValue(200.0);
  temperatureLimits.setNumericType("CONTINUOUS");
  temperatureLimits.setUnitType("Temperature");

  DefaultScheduleSet defaultScheduleSet(model);
  defaultScheduleSet.setName("Default Schedules");

  ScheduleRuleset numberOfPeopleSchedule(model);
  numberOfPeopleSchedule.setName("Medium Office Number of People Schedule");
  numberOfPeopleSchedule.defaultDaySchedule().setName("Medium Office Number of People All Other Days Schedule");
  ScheduleDay numberOfPeopleSummerDesignDay(model);
  numberOfPeopleSummerDesignDay.setName("Medium Office Number of People Summer Design Day Schedule");
  numberOfPeopleSummerDesignDay.clearValues();
  numberOfPeopleSummerDesignDay.addValue(Time(0,6,0,0),0.0);
  numberOfPeopleSummerDesignDay.addValue(Time(0,22,0,0),1.0);
  numberOfPeopleSummerDesignDay.addValue(Time(0,24,0,0),0.05);
  numberOfPeopleSchedule.setSummerDesignDaySchedule(numberOfPeopleSummerDesignDay);
  ScheduleRule numberOfPeopleWeekdaysRule(numberOfPeopleSchedule);
  numberOfPeopleWeekdaysRule.setName("Medium Office Number of People Schedule Weekdays Rule");
  numberOfPeopleWeekdaysRule.setApplyMonday(true);
  numberOfPeopleWeekdaysRule.setApplyTuesday(true);
  numberOfPeopleWeekdaysRule.setApplyWednesday(true);
  numberOfPeopleWeekdaysRule.setApplyThursday(true);
  numberOfPeopleWeekdaysRule.setApplyFriday(true);
  numberOfPeopleWeekdaysRule.daySchedule().setName("Medium Office Number of People Weekdays Schedule");
  numberOfPeopleWeekdaysRule.daySchedule().clearValues();
  numberOfPeopleWeekdaysRule.daySchedule().addValue(Time(0,6,0,0),0.0);
  numberOfPeopleWeekdaysRule.daySchedule().addValue(Time(0,7,0,0),0.1);
  numberOfPeopleWeekdaysRule.daySchedule().addValue(Time(0,8,0,0),0.2);
  numberOfPeopleWeekdaysRule.daySchedule().addValue(Time(0,12,0,0),0.95);
  numberOfPeopleWeekdaysRule.daySchedule().addValue(Time(0,13,0,0),0.5);
  numberOfPeopleWeekdaysRule.daySchedule().addValue(Time(0,17,0,0),0.95);
  numberOfPeopleWeekdaysRule.daySchedule().addValue(Time(0,18,0,0),0.7);
  numberOfPeopleWeekdaysRule.daySchedule().addValue(Time(0,20,0,0),0.4);
  numberOfPeopleWeekdaysRule.daySchedule().addValue(Time(0,22,0,0),0.1);
  numberOfPeopleWeekdaysRule.daySchedule().addValue(Time(0,24,0,0),0.05);
  ScheduleRule numberOfPeopleSaturdayRule(numberOfPeopleSchedule);
  numberOfPeopleSaturdayRule.setName("Medium Office Number of People Schedule Saturday Rule");
  numberOfPeopleSaturdayRule.setApplySaturday(true);
  numberOfPeopleSaturdayRule.daySchedule().setName("Medium Office Number of People Saturday Schedule");
  numberOfPeopleSaturdayRule.daySchedule().clearValues();
  numberOfPeopleSaturdayRule.daySchedule().addValue(Time(0,6,0,0),0.0);
  numberOfPeopleSaturdayRule.daySchedule().addValue(Time(0,8,0,0),0.1);
  numberOfPeopleSaturdayRule.daySchedule().addValue(Time(0,14,0,0),0.5);
  numberOfPeopleSaturdayRule.daySchedule().addValue(Time(0,17,0,0),0.1);
  numberOfPeopleSaturdayRule.daySchedule().addValue(Time(0,24,0,0),0.0);

  defaultScheduleSet.setNumberofPeopleSchedule(numberOfPeopleSchedule);

  ScheduleRuleset peopleActivityLevelSchedule(model);
  peopleActivityLevelSchedule.setName("Medium Office People Activity Level Schedule");
  peopleActivityLevelSchedule.defaultDaySchedule().setName("Medium Office People Activity Level All Days Schedule");
  peopleActivityLevelSchedule.defaultDaySchedule().clearValues();
  peopleActivityLevelSchedule.defaultDaySchedule().addValue(Time(0,24,0,0),120.0);

  defaultScheduleSet.setPeopleActivityLevelSchedule(peopleActivityLevelSchedule);

  ScheduleRuleset lightingSchedule(model);
  lightingSchedule.setName("Medium Office Lighting Schedule");
  lightingSchedule.defaultDaySchedule().setName("Medium Office Lighting All Other Days Schedule");
  lightingSchedule.defaultDaySchedule().clearValues();
  lightingSchedule.defaultDaySchedule().addValue(Time(0,24,0,0),0.05);
  ScheduleDay lightingSummerDesignDay(model);
  lightingSummerDesignDay.setName("Medium Office Lighting Summer Design Day Schedule");
  lightingSummerDesignDay.clearValues();
  lightingSummerDesignDay.addValue(Time(0,6,0,0),1.0);
  lightingSchedule.setSummerDesignDaySchedule(lightingSummerDesignDay);
  ScheduleDay lightingWinterDesignDay(model);
  lightingWinterDesignDay.setName("Medium Office Lighting Winter Design Day Schedule");
  lightingSchedule.setWinterDesignDaySchedule(lightingWinterDesignDay);
  ScheduleRule lightingWeekdaysRule(lightingSchedule);
  lightingWeekdaysRule.setName("Medium Office Lighting Schedule Weekdays Rule");
  lightingWeekdaysRule.setApplyMonday(true);
  lightingWeekdaysRule.setApplyTuesday(true);
  lightingWeekdaysRule.setApplyWednesday(true);
  lightingWeekdaysRule.setApplyThursday(true);
  lightingWeekdaysRule.setApplyFriday(true);
  lightingWeekdaysRule.daySchedule().setName("Medium Office Lighting Weekdays Schedule");
  lightingWeekdaysRule.daySchedule().clearValues();
  lightingWeekdaysRule.daySchedule().addValue(Time(0,5,0,0),0.05);
  lightingWeekdaysRule.daySchedule().addValue(Time(0,7,0,0),0.1);
  lightingWeekdaysRule.daySchedule().addValue(Time(0,8,0,0),0.3);
  lightingWeekdaysRule.daySchedule().addValue(Time(0,17,0,0),0.9);
  lightingWeekdaysRule.daySchedule().addValue(Time(0,18,0,0),0.7);
  lightingWeekdaysRule.daySchedule().addValue(Time(0,20,0,0),0.5);
  lightingWeekdaysRule.daySchedule().addValue(Time(0,22,0,0),0.3);
  lightingWeekdaysRule.daySchedule().addValue(Time(0,23,0,0),0.1);
  lightingWeekdaysRule.daySchedule().addValue(Time(0,24,0,0),0.05);
  ScheduleRule lightingSaturdayRule(lightingSchedule);
  lightingSaturdayRule.setName("Medium Office Lighting Schedule Saturday Rule");
  lightingSaturdayRule.setApplySaturday(true);
  lightingSaturdayRule.daySchedule().setName("Medium Office Lighting Saturday Schedule");
  lightingSaturdayRule.daySchedule().clearValues();
  lightingSaturdayRule.daySchedule().addValue(Time(0,6,0,0),0.05);
  lightingSaturdayRule.daySchedule().addValue(Time(0,8,0,0),0.1);
  lightingSaturdayRule.daySchedule().addValue(Time(0,14,0,0),0.5);
  lightingSaturdayRule.daySchedule().addValue(Time(0,17,0,0),0.15);
  lightingSaturdayRule.daySchedule().addValue(Time(0,24,0,0),0.05);

  defaultScheduleSet.setLightingSchedule(lightingSchedule);

  ScheduleRuleset electricEquipmentSchedule(model);
  electricEquipmentSchedule.setName("Medium Office Electric Equipment Schedule");
  electricEquipmentSchedule.defaultDaySchedule().setName("Medium Office Electric Equipment All Other Days Schedule");
  electricEquipmentSchedule.defaultDaySchedule().clearValues();
  electricEquipmentSchedule.defaultDaySchedule().addValue(Time(0,24,0,0),0.3);
  ScheduleDay electricEquipmentSummerDesignDay(model);
  electricEquipmentSummerDesignDay.setName("Medium Office Electric Equipment Summer Design Day Schedule");
  electricEquipmentSummerDesignDay.clearValues();
  electricEquipmentSummerDesignDay.addValue(Time(0,6,0,0),1.0);
  electricEquipmentSchedule.setSummerDesignDaySchedule(electricEquipmentSummerDesignDay);
  ScheduleDay electricEquipmentWinterDesignDay(model);
  electricEquipmentWinterDesignDay.setName("Medium Office Electric Equipment Winter Design Day Schedule");
  electricEquipmentSchedule.setWinterDesignDaySchedule(electricEquipmentWinterDesignDay);
  ScheduleRule electricEquipmentWeekdaysRule(electricEquipmentSchedule);
  electricEquipmentWeekdaysRule.setName("Medium Office Electric Equipment Schedule Weekdays Rule");
  electricEquipmentWeekdaysRule.setApplyMonday(true);
  electricEquipmentWeekdaysRule.setApplyTuesday(true);
  electricEquipmentWeekdaysRule.setApplyWednesday(true);
  electricEquipmentWeekdaysRule.setApplyThursday(true);
  electricEquipmentWeekdaysRule.setApplyFriday(true);
  electricEquipmentWeekdaysRule.daySchedule().setName("Medium Office Electric Equipment Weekdays Schedule");
  electricEquipmentWeekdaysRule.daySchedule().clearValues();
  electricEquipmentWeekdaysRule.daySchedule().addValue(Time(0,8,0,0),0.4);
  electricEquipmentWeekdaysRule.daySchedule().addValue(Time(0,12,0,0),0.9);
  electricEquipmentWeekdaysRule.daySchedule().addValue(Time(0,13,0,0),0.8);
  electricEquipmentWeekdaysRule.daySchedule().addValue(Time(0,17,0,0),0.9);
  electricEquipmentWeekdaysRule.daySchedule().addValue(Time(0,18,0,0),0.8);
  electricEquipmentWeekdaysRule.daySchedule().addValue(Time(0,20,0,0),0.6);
  electricEquipmentWeekdaysRule.daySchedule().addValue(Time(0,22,0,0),0.5);
  electricEquipmentWeekdaysRule.daySchedule().addValue(Time(0,24,0,0),0.4);
  ScheduleRule electricEquipmentSaturdayRule(electricEquipmentSchedule);
  electricEquipmentSaturdayRule.setName("Medium Office Electric Equipment Schedule Saturday Rule");
  electricEquipmentSaturdayRule.setApplySaturday(true);
  electricEquipmentSaturdayRule.daySchedule().setName("Medium Office Electric Equipment Saturday Schedule");
  electricEquipmentSaturdayRule.daySchedule().clearValues();
  electricEquipmentSaturdayRule.daySchedule().addValue(Time(0,6,0,0),0.3);
  electricEquipmentSaturdayRule.daySchedule().addValue(Time(0,8,0,0),0.4);
  electricEquipmentSaturdayRule.daySchedule().addValue(Time(0,14,0,0),0.5);
  electricEquipmentSaturdayRule.daySchedule().addValue(Time(0,17,0,0),0.35);
  electricEquipmentSaturdayRule.daySchedule().addValue(Time(0,24,0,0),0.3);

  defaultScheduleSet.setElectricEquipmentSchedule(electricEquipmentSchedule);

  ScheduleRuleset gasEquipmentSchedule(model);
  gasEquipmentSchedule.setName("Medium Office Gas Equipment Schedule");
  gasEquipmentSchedule.defaultDaySchedule().setName("Medium Office Gas Equipment All Other Days Schedule");
  gasEquipmentSchedule.defaultDaySchedule().clearValues();
  gasEquipmentSchedule.defaultDaySchedule().addValue(Time(0,24,0,0),0.3);
  ScheduleDay gasEquipmentSummerDesignDay(model);
  gasEquipmentSummerDesignDay.setName("Medium Office Gas Equipment Summer Design Day Schedule");
  gasEquipmentSummerDesignDay.clearValues();
  gasEquipmentSummerDesignDay.addValue(Time(0,6,0,0),1.0);
  gasEquipmentSchedule.setSummerDesignDaySchedule(gasEquipmentSummerDesignDay);
  ScheduleDay gasEquipmentWinterDesignDay(model);
  gasEquipmentWinterDesignDay.setName("Medium Office Gas Equipment Winter Design Day Schedule");
  gasEquipmentSchedule.setWinterDesignDaySchedule(gasEquipmentWinterDesignDay);
  ScheduleRule gasEquipmentWeekdaysRule(gasEquipmentSchedule);
  gasEquipmentWeekdaysRule.setName("Medium Office Gas Equipment Schedule Weekdays Rule");
  gasEquipmentWeekdaysRule.setApplyMonday(true);
  gasEquipmentWeekdaysRule.setApplyTuesday(true);
  gasEquipmentWeekdaysRule.setApplyWednesday(true);
  gasEquipmentWeekdaysRule.setApplyThursday(true);
  gasEquipmentWeekdaysRule.setApplyFriday(true);
  gasEquipmentWeekdaysRule.daySchedule().setName("Medium Office Gas Equipment Weekdays Schedule");
  gasEquipmentWeekdaysRule.daySchedule().clearValues();
  gasEquipmentWeekdaysRule.daySchedule().addValue(Time(0,8,0,0),0.4);
  gasEquipmentWeekdaysRule.daySchedule().addValue(Time(0,12,0,0),0.9);
  gasEquipmentWeekdaysRule.daySchedule().addValue(Time(0,13,0,0),0.8);
  gasEquipmentWeekdaysRule.daySchedule().addValue(Time(0,17,0,0),0.9);
  gasEquipmentWeekdaysRule.daySchedule().addValue(Time(0,18,0,0),0.8);
  gasEquipmentWeekdaysRule.daySchedule().addValue(Time(0,20,0,0),0.6);
  gasEquipmentWeekdaysRule.daySchedule().addValue(Time(0,22,0,0),0.5);
  gasEquipmentWeekdaysRule.daySchedule().addValue(Time(0,24,0,0),0.4);
  ScheduleRule gasEquipmentSaturdayRule(gasEquipmentSchedule);
  gasEquipmentSaturdayRule.setName("Medium Office Gas Equipment Schedule Saturday Rule");
  gasEquipmentSaturdayRule.setApplySaturday(true);
  gasEquipmentSaturdayRule.daySchedule().setName("Medium Office Gas Equipment Saturday Schedule");
  gasEquipmentSaturdayRule.daySchedule().clearValues();
  gasEquipmentSaturdayRule.daySchedule().addValue(Time(0,6,0,0),0.3);
  gasEquipmentSaturdayRule.daySchedule().addValue(Time(0,8,0,0),0.4);
  gasEquipmentSaturdayRule.daySchedule().addValue(Time(0,14,0,0),0.5);
  gasEquipmentSaturdayRule.daySchedule().addValue(Time(0,17,0,0),0.35);
  gasEquipmentSaturdayRule.daySchedule().addValue(Time(0,24,0,0),0.3);

  defaultScheduleSet.setGasEquipmentSchedule(gasEquipmentSchedule);

  ScheduleRuleset hotWaterEquipmentSchedule(model);
  hotWaterEquipmentSchedule.setName("Medium Office Hot Water Equipment Schedule");
  hotWaterEquipmentSchedule.defaultDaySchedule().setName("Medium Office Hot Water Equipment Default Schedule");
  hotWaterEquipmentSchedule.defaultDaySchedule().clearValues();
  hotWaterEquipmentSchedule.defaultDaySchedule().addValue(Time(0,5,0,0),0.04);
  hotWaterEquipmentSchedule.defaultDaySchedule().addValue(Time(0,6,0,0),0.07);
  hotWaterEquipmentSchedule.defaultDaySchedule().addValue(Time(0,11,0,0),0.04);
  hotWaterEquipmentSchedule.defaultDaySchedule().addValue(Time(0,13,0,0),0.06);
  hotWaterEquipmentSchedule.defaultDaySchedule().addValue(Time(0,14,0,0),0.09);
  hotWaterEquipmentSchedule.defaultDaySchedule().addValue(Time(0,15,0,0),0.06);
  hotWaterEquipmentSchedule.defaultDaySchedule().addValue(Time(0,21,0,0),0.04);
  hotWaterEquipmentSchedule.defaultDaySchedule().addValue(Time(0,22,0,0),0.07);
  hotWaterEquipmentSchedule.defaultDaySchedule().addValue(Time(0,24,0,0),0.04);
  ScheduleDay hotWaterEquipmentSummerDesignDay(model);
  hotWaterEquipmentSummerDesignDay.setName("Medium Office Hot Water Equipment Summer Design Day Schedule");
  hotWaterEquipmentSummerDesignDay.clearValues();
  hotWaterEquipmentSummerDesignDay.addValue(Time(0,5,0,0),0.05);
  hotWaterEquipmentSummerDesignDay.addValue(Time(0,6,0,0),0.08);
  hotWaterEquipmentSummerDesignDay.addValue(Time(0,7,0,0),0.07);
  hotWaterEquipmentSummerDesignDay.addValue(Time(0,8,0,0),0.19);
  hotWaterEquipmentSummerDesignDay.addValue(Time(0,9,0,0),0.35);
  hotWaterEquipmentSummerDesignDay.addValue(Time(0,10,0,0),0.38);
  hotWaterEquipmentSummerDesignDay.addValue(Time(0,11,0,0),0.39);
  hotWaterEquipmentSummerDesignDay.addValue(Time(0,12,0,0),0.47);
  hotWaterEquipmentSummerDesignDay.addValue(Time(0,13,0,0),0.57);
  hotWaterEquipmentSummerDesignDay.addValue(Time(0,14,0,0),0.54);
  hotWaterEquipmentSummerDesignDay.addValue(Time(0,15,0,0),0.34);
  hotWaterEquipmentSummerDesignDay.addValue(Time(0,16,0,0),0.33);
  hotWaterEquipmentSummerDesignDay.addValue(Time(0,17,0,0),0.44);
  hotWaterEquipmentSummerDesignDay.addValue(Time(0,18,0,0),0.26);
  hotWaterEquipmentSummerDesignDay.addValue(Time(0,19,0,0),0.21);
  hotWaterEquipmentSummerDesignDay.addValue(Time(0,20,0,0),0.15);
  hotWaterEquipmentSummerDesignDay.addValue(Time(0,21,0,0),0.17);
  hotWaterEquipmentSummerDesignDay.addValue(Time(0,22,0,0),0.08);
  hotWaterEquipmentSummerDesignDay.addValue(Time(0,24,0,0),0.05);
  hotWaterEquipmentSchedule.setSummerDesignDaySchedule(hotWaterEquipmentSummerDesignDay);
  ScheduleRule hotWaterEquipmentWeekdaysRule(hotWaterEquipmentSchedule,hotWaterEquipmentSummerDesignDay);
  hotWaterEquipmentWeekdaysRule.setName("Medium Office Hot Water Schedule Weekdays Rule");
  hotWaterEquipmentWeekdaysRule.setApplyMonday(true);
  hotWaterEquipmentWeekdaysRule.setApplyTuesday(true);
  hotWaterEquipmentWeekdaysRule.setApplyWednesday(true);
  hotWaterEquipmentWeekdaysRule.setApplyThursday(true);
  hotWaterEquipmentWeekdaysRule.setApplyFriday(true);
  hotWaterEquipmentWeekdaysRule.daySchedule().setName("Medium Office Hot Water Equipment Weekdays Schedule");
  ScheduleDay hotWaterEquipmentWinterDesignDay(model);
  hotWaterEquipmentWinterDesignDay.setName("Medium Office Hot Water Equipment Winter Design Day Schedule");
  hotWaterEquipmentWinterDesignDay.clearValues();
  hotWaterEquipmentWinterDesignDay.addValue(Time(0,5,0,0),0.05);
  hotWaterEquipmentWinterDesignDay.addValue(Time(0,6,0,0),0.08);
  hotWaterEquipmentWinterDesignDay.addValue(Time(0,7,0,0),0.07);
  hotWaterEquipmentWinterDesignDay.addValue(Time(0,8,0,0),0.11);
  hotWaterEquipmentWinterDesignDay.addValue(Time(0,9,0,0),0.15);
  hotWaterEquipmentWinterDesignDay.addValue(Time(0,10,0,0),0.21);
  hotWaterEquipmentWinterDesignDay.addValue(Time(0,11,0,0),0.19);
  hotWaterEquipmentWinterDesignDay.addValue(Time(0,12,0,0),0.23);
  hotWaterEquipmentWinterDesignDay.addValue(Time(0,13,0,0),0.20);
  hotWaterEquipmentWinterDesignDay.addValue(Time(0,14,0,0),0.19);
  hotWaterEquipmentWinterDesignDay.addValue(Time(0,15,0,0),0.15);
  hotWaterEquipmentWinterDesignDay.addValue(Time(0,16,0,0),0.13);
  hotWaterEquipmentWinterDesignDay.addValue(Time(0,17,0,0),0.14);
  hotWaterEquipmentWinterDesignDay.addValue(Time(0,21,0,0),0.07);
  hotWaterEquipmentWinterDesignDay.addValue(Time(0,22,0,0),0.09);
  hotWaterEquipmentWinterDesignDay.addValue(Time(0,24,0,0),0.05);
  hotWaterEquipmentSchedule.setWinterDesignDaySchedule(hotWaterEquipmentWinterDesignDay);
  ScheduleRule hotWaterEquipmentSaturdayRule(hotWaterEquipmentSchedule,hotWaterEquipmentWinterDesignDay);
  hotWaterEquipmentSaturdayRule.setName("Medium Office Hot Water Schedule Saturday Rule");
  hotWaterEquipmentSaturdayRule.setApplySaturday(true);
  hotWaterEquipmentSaturdayRule.daySchedule().setName("Medium Office Hot Water Equipment Saturday Schedule");

  defaultScheduleSet.setHotWaterEquipmentSchedule(hotWaterEquipmentSchedule);

  ScheduleRuleset infiltrationSchedule(model);
  infiltrationSchedule.setName("Medium Office Infiltration Schedule");
  infiltrationSchedule.defaultDaySchedule().setName("Medium Office Infiltration Default Schedule");
  infiltrationSchedule.defaultDaySchedule().clearValues();
  infiltrationSchedule.defaultDaySchedule().addValue(Time(0,24,0,0),1.0);
  ScheduleDay infiltrationSummerDesignDay(model);
  infiltrationSummerDesignDay.setName("Medium Office Infiltration Summer Design Day Schedule");
  infiltrationSummerDesignDay.clearValues();
  infiltrationSummerDesignDay.addValue(Time(0,6,0,0),1.0);
  infiltrationSummerDesignDay.addValue(Time(0,22,0,0),0.25);
  infiltrationSummerDesignDay.addValue(Time(0,24,0,0),1.0);
  infiltrationSchedule.setSummerDesignDaySchedule(infiltrationSummerDesignDay);
  ScheduleRule infiltrationWeekdaysRule(infiltrationSchedule,infiltrationSummerDesignDay);
  infiltrationWeekdaysRule.setName("Medium Office Infiltration Schedule Weekdays Rule");
  infiltrationWeekdaysRule.setApplyMonday(true);
  infiltrationWeekdaysRule.setApplyTuesday(true);
  infiltrationWeekdaysRule.setApplyWednesday(true);
  infiltrationWeekdaysRule.setApplyThursday(true);
  infiltrationWeekdaysRule.setApplyFriday(true);
  infiltrationWeekdaysRule.daySchedule().setName("Medium Office Infiltration Weekdays Schedule");
  ScheduleDay infiltrationWinterDesignDay(model);
  infiltrationWinterDesignDay.setName("Medium Office Infiltration Winter Design Day Schedule");
  infiltrationWinterDesignDay.clearValues();
  infiltrationWinterDesignDay.addValue(Time(0,6,0,0),1.0);
  infiltrationWinterDesignDay.addValue(Time(0,18,0,0),0.25);
  infiltrationWinterDesignDay.addValue(Time(0,24,0,0),1.0);
  infiltrationSchedule.setWinterDesignDaySchedule(infiltrationWinterDesignDay);
  ScheduleRule infiltrationSaturdayRule(infiltrationSchedule,infiltrationWinterDesignDay);
  infiltrationSaturdayRule.setName("Medium Office Infiltration Schedule Saturday Rule");
  infiltrationSaturdayRule.setApplySaturday(true);
  infiltrationSaturdayRule.daySchedule().setName("Medium Office Infiltration Saturday Schedule");

  defaultScheduleSet.setInfiltrationSchedule(infiltrationSchedule);

  ScheduleRuleset coolingSetpointSchedule(model);
  coolingSetpointSchedule.setName("Medium Office Cooling Setpoint Schedule");
  coolingSetpointSchedule.setScheduleTypeLimits(temperatureLimits);
  coolingSetpointSchedule.defaultDaySchedule().setName("Medium Office Cooling Setpoint All Other Days Schedule");
  coolingSetpointSchedule.defaultDaySchedule().clearValues();
  coolingSetpointSchedule.defaultDaySchedule().addValue(Time(0,24,0,0),26.7);
  ScheduleDay coolingSetpointSummerDesignDay(model);
  coolingSetpointSummerDesignDay.setName("Medium Office Cooling Setpoint Summer Design Day Schedule");
  coolingSetpointSummerDesignDay.setScheduleTypeLimits(temperatureLimits);
  coolingSetpointSummerDesignDay.clearValues();
  coolingSetpointSummerDesignDay.addValue(Time(0,6,0,0),26.7);
  coolingSetpointSummerDesignDay.addValue(Time(0,22,0,0),24.0);
  coolingSetpointSummerDesignDay.addValue(Time(0,24,0,0),26.7);
  coolingSetpointSchedule.setSummerDesignDaySchedule(coolingSetpointSummerDesignDay);
  ScheduleRule coolingSetpointWeekdaysRule(coolingSetpointSchedule,coolingSetpointSummerDesignDay);
  coolingSetpointWeekdaysRule.setName("Medium Office Cooling Setpoint Schedule Weekdays Rule");
  coolingSetpointWeekdaysRule.setApplyMonday(true);
  coolingSetpointWeekdaysRule.setApplyTuesday(true);
  coolingSetpointWeekdaysRule.setApplyWednesday(true);
  coolingSetpointWeekdaysRule.setApplyThursday(true);
  coolingSetpointWeekdaysRule.setApplyFriday(true);
  coolingSetpointWeekdaysRule.daySchedule().setName("Medium Office Cooling Setpoint Weekdays Schedule");
  ScheduleRule coolingSetpointSaturdayRule(coolingSetpointSchedule);
  coolingSetpointSaturdayRule.setName("Medium Office Cooling Setpoint Schedule Saturday Rule");
  coolingSetpointSaturdayRule.setApplySaturday(true);
  coolingSetpointSaturdayRule.daySchedule().setName("Medium Office Cooling Setpoint Saturday Schedule");
  coolingSetpointSaturdayRule.daySchedule().clearValues();
  coolingSetpointSaturdayRule.daySchedule().addValue(Time(0,6,0,0),26.7);
  coolingSetpointSaturdayRule.daySchedule().addValue(Time(0,18,0,0),24.0);
  coolingSetpointSaturdayRule.daySchedule().addValue(Time(0,24,0,0),26.7);

  ScheduleRuleset heatingSetpointSchedule(model);
  heatingSetpointSchedule.setName("Medium Office Heating Setpoint Schedule");
  heatingSetpointSchedule.setScheduleTypeLimits(temperatureLimits);
  heatingSetpointSchedule.defaultDaySchedule().setName("Medium Office Heating Setpoint All Other Days Schedule");
  heatingSetpointSchedule.defaultDaySchedule().clearValues();
  heatingSetpointSchedule.defaultDaySchedule().addValue(Time(0,24,0,0),15.6);
  ScheduleDay heatingSetpointWinterDesignDay(model);
  heatingSetpointWinterDesignDay.setName("Medium Office Heating Setpoint Winter Design Day Schedule");
  heatingSetpointWinterDesignDay.setScheduleTypeLimits(temperatureLimits);
  heatingSetpointWinterDesignDay.clearValues();
  heatingSetpointWinterDesignDay.addValue(Time(0,24,0,0),21.0);
  heatingSetpointSchedule.setWinterDesignDaySchedule(heatingSetpointWinterDesignDay);
  ScheduleRule heatingSetpointWeekdaysRule(heatingSetpointSchedule);
  heatingSetpointWeekdaysRule.setName("Medium Office Heating Setpoint Schedule Weekdays Rule");
  heatingSetpointWeekdaysRule.setApplyMonday(true);
  heatingSetpointWeekdaysRule.setApplyTuesday(true);
  heatingSetpointWeekdaysRule.setApplyWednesday(true);
  heatingSetpointWeekdaysRule.setApplyThursday(true);
  heatingSetpointWeekdaysRule.setApplyFriday(true);
  heatingSetpointWeekdaysRule.daySchedule().setName("Medium Office Heating Setpoint Weekdays Schedule");
  heatingSetpointWeekdaysRule.daySchedule().clearValues();
  heatingSetpointWeekdaysRule.daySchedule().addValue(Time(0,6,0,0),15.6);
  heatingSetpointWeekdaysRule.daySchedule().addValue(Time(0,22,0,0),21.0);
  heatingSetpointWeekdaysRule.daySchedule().addValue(Time(0,24,0,0),15.6);
  ScheduleRule heatingSetpointSaturdayRule(heatingSetpointSchedule);
  heatingSetpointSaturdayRule.setName("Medium Office Heating Setpoint Schedule Saturday Rule");
  heatingSetpointSaturdayRule.setApplySaturday(true);
  heatingSetpointSaturdayRule.daySchedule().setName("Medium Office Heating Setpoint Saturday Schedule");
  heatingSetpointSaturdayRule.daySchedule().clearValues();
  heatingSetpointSaturdayRule.daySchedule().addValue(Time(0,6,0,0),15.6);
  heatingSetpointSaturdayRule.daySchedule().addValue(Time(0,18,0,0),21.0);
  heatingSetpointSaturdayRule.daySchedule().addValue(Time(0,24,0,0),15.6);
}

void addExampleConstructions(Model& model)
{

  DefaultConstructionSet defaultConstructions(model);
  defaultConstructions.setName("Default Constructions");
  DefaultSurfaceConstructions exteriorSurfaceConstructions(model);
  exteriorSurfaceConstructions.setName("Exterior Surface Constructions");
  defaultConstructions.setDefaultExteriorSurfaceConstructions(exteriorSurfaceConstructions);
  DefaultSurfaceConstructions interiorSurfaceConstructions(model);
  interiorSurfaceConstructions.setName("Interior Surface Constructions");
  defaultConstructions.setDefaultInteriorSurfaceConstructions(interiorSurfaceConstructions);
  DefaultSurfaceConstructions groundContactSurfaceConstructions(model);
  groundContactSurfaceConstructions.setName("Ground Contact Surface Constructions");
  defaultConstructions.setDefaultGroundContactSurfaceConstructions(groundContactSurfaceConstructions);
  DefaultSubSurfaceConstructions exteriorSubSurfaceConstructions(model);
  exteriorSubSurfaceConstructions.setName("Exterior SubSurface Constructions");
  defaultConstructions.setDefaultExteriorSubSurfaceConstructions(exteriorSubSurfaceConstructions);
  DefaultSubSurfaceConstructions interiorSubSurfaceConstructions(model);
  interiorSubSurfaceConstructions.setName("Interior SubSurface Constructions");
  defaultConstructions.setDefaultInteriorSubSurfaceConstructions(interiorSubSurfaceConstructions);

  OpaqueMaterialVector opaqueMaterials;

  // Exterior Wall

  StandardOpaqueMaterial m01_100mm_brick(model);
  m01_100mm_brick.setName("M01 100mm brick");
  m01_100mm_brick.setRoughness("MediumRough");
  m01_100mm_brick.setThickness(0.1016);
  m01_100mm_brick.setThermalConductivity(0.89);
  m01_100mm_brick.setDensity(1920.0);
  m01_100mm_brick.setSpecificHeat(790.0);

  opaqueMaterials.push_back(m01_100mm_brick);

  StandardOpaqueMaterial m15_200mm_heavyweight_concrete(model);
  m15_200mm_heavyweight_concrete.setName("M15 200mm heavyweight concrete");
  m15_200mm_heavyweight_concrete.setRoughness("MediumRough");
  m15_200mm_heavyweight_concrete.setThickness(0.2032);
  m15_200mm_heavyweight_concrete.setThermalConductivity(1.95);
  m15_200mm_heavyweight_concrete.setDensity(2240.0);
  m15_200mm_heavyweight_concrete.setSpecificHeat(900.0);

  opaqueMaterials.push_back(m15_200mm_heavyweight_concrete);

  StandardOpaqueMaterial i02_50mm_insulation_board(model);
  i02_50mm_insulation_board.setName("I02 50mm insulation board");
  i02_50mm_insulation_board.setRoughness("MediumRough");
  i02_50mm_insulation_board.setThickness(0.0508);
  i02_50mm_insulation_board.setThermalConductivity(0.03);
  i02_50mm_insulation_board.setDensity(43.0);
  i02_50mm_insulation_board.setSpecificHeat(1210.0);

  opaqueMaterials.push_back(i02_50mm_insulation_board);

  AirGap f04_wall_air_space_resistance(model);
  f04_wall_air_space_resistance.setName("F04 Wall air space resistance");
  f04_wall_air_space_resistance.setThermalResistance(0.15);

  opaqueMaterials.push_back(f04_wall_air_space_resistance);

  StandardOpaqueMaterial g01a_19mm_gypsum_board(model);
  g01a_19mm_gypsum_board.setName("G01a 19mm gypsum board");
  g01a_19mm_gypsum_board.setRoughness("MediumSmooth");
  g01a_19mm_gypsum_board.setThickness(0.019);
  g01a_19mm_gypsum_board.setThermalConductivity(0.16);
  g01a_19mm_gypsum_board.setDensity(800.0);
  g01a_19mm_gypsum_board.setSpecificHeat(1090.0);

  opaqueMaterials.push_back(g01a_19mm_gypsum_board);

  Construction exteriorWall(opaqueMaterials);
  exteriorWall.setName("Exterior Wall");
  exteriorWall.setInsulation(i02_50mm_insulation_board);
  exteriorSurfaceConstructions.setWallConstruction(exteriorWall);
  opaqueMaterials.clear();

  // Exterior Roof

  StandardOpaqueMaterial m11_100mm_lightweight_concrete(model);
  m11_100mm_lightweight_concrete.setName("M11 100mm lightweight concrete");
  m11_100mm_lightweight_concrete.setRoughness("MediumRough");
  m11_100mm_lightweight_concrete.setThickness(0.1016);
  m11_100mm_lightweight_concrete.setThermalConductivity(0.53);
  m11_100mm_lightweight_concrete.setDensity(1280.0);
  m11_100mm_lightweight_concrete.setSpecificHeat(840.0);

  opaqueMaterials.push_back(m11_100mm_lightweight_concrete);

  AirGap f05_ceiling_air_space_resistance(model);
  f05_ceiling_air_space_resistance.setName("F05 Ceiling air space resistance");
  f05_ceiling_air_space_resistance.setThermalResistance(0.18);

  opaqueMaterials.push_back(f05_ceiling_air_space_resistance);

  StandardOpaqueMaterial f16_acoustic_tile(model);
  f16_acoustic_tile.setName("F16 Acoustic tile");
  f16_acoustic_tile.setRoughness("MediumSmooth");
  f16_acoustic_tile.setThickness(0.0191);
  f16_acoustic_tile.setThermalConductivity(0.06);
  f16_acoustic_tile.setDensity(368.0);
  f16_acoustic_tile.setSpecificHeat(590.0);

  opaqueMaterials.push_back(f16_acoustic_tile);

  Construction exteriorRoof(opaqueMaterials);
  exteriorRoof.setName("Exterior Roof");
  exteriorSurfaceConstructions.setRoofCeilingConstruction(exteriorRoof);
  opaqueMaterials.clear();

  // Interior Floor

  opaqueMaterials.push_back(f16_acoustic_tile);
  opaqueMaterials.push_back(f05_ceiling_air_space_resistance);
  opaqueMaterials.push_back(m11_100mm_lightweight_concrete);

  Construction interiorFloor(opaqueMaterials);
  interiorFloor.setName("Interior Floor");
  interiorSurfaceConstructions.setFloorConstruction(interiorFloor);
  opaqueMaterials.clear();

  // Air Wall

  AirWallMaterial airWallMaterial(model);
  airWallMaterial.setName("Air Wall Material");

  Construction airWall(airWallMaterial);
  airWall.setName("Air Wall");
  interiorSurfaceConstructions.setWallConstruction(airWall);

  // Interior Ceiling

  opaqueMaterials.push_back(m11_100mm_lightweight_concrete);
  opaqueMaterials.push_back(f05_ceiling_air_space_resistance);
  opaqueMaterials.push_back(f16_acoustic_tile);

  Construction interiorCeiling(opaqueMaterials);
  interiorCeiling.setName("Interior Ceiling");
  interiorSurfaceConstructions.setRoofCeilingConstruction(interiorCeiling);
  opaqueMaterials.clear();

  // Slab

  StandardOpaqueMaterial mat_cc05_8_hw_concrete(model);
  mat_cc05_8_hw_concrete.setName("MAT-CC05 8 HW CONCRETE");
  mat_cc05_8_hw_concrete.setRoughness("Rough");
  mat_cc05_8_hw_concrete.setThickness(0.2032);
  mat_cc05_8_hw_concrete.setThermalConductivity(1.3110);
  mat_cc05_8_hw_concrete.setDensity(2240.0);
  mat_cc05_8_hw_concrete.setSpecificHeat(836.8);
  mat_cc05_8_hw_concrete.setThermalAbsorptance(0.9);
  mat_cc05_8_hw_concrete.setSolarAbsorptance(0.7);
  mat_cc05_8_hw_concrete.setVisibleAbsorptance(0.7);

  opaqueMaterials.push_back(mat_cc05_8_hw_concrete);

  MasslessOpaqueMaterial cp02_carpet_pad(model);
  cp02_carpet_pad.setName("CP02 CARPET PAD");
  cp02_carpet_pad.setRoughness("VeryRough");
  cp02_carpet_pad.setThermalResistance(0.2165);
  cp02_carpet_pad.setThermalAbsorptance(0.9);
  cp02_carpet_pad.setSolarAbsorptance(0.7);
  cp02_carpet_pad.setVisibleAbsorptance(0.8);

  opaqueMaterials.push_back(cp02_carpet_pad);

  Construction slab(opaqueMaterials);
  slab.setName("Slab");
  groundContactSurfaceConstructions.setFloorConstruction(slab);
  opaqueMaterials.clear();

  FenestrationMaterialVector fenestrationMaterials;

  // Exterior Window

  SimpleGlazing simple_glazing(model);
  simple_glazing.setName("Simple Glazing");
  simple_glazing.setUFactor(3.23646);
  simple_glazing.setSolarHeatGainCoefficient(0.39);
  simple_glazing.setVisibleTransmittance(0.6);

  StandardGlazing clear_3mm(model);
  clear_3mm.setName("Clear 3mm");
  clear_3mm.setOpticalDataType("SpectralAverage");
  clear_3mm.setThickness(0.003);
  clear_3mm.setSolarTransmittance(0.837);
  clear_3mm.setFrontSideSolarReflectanceatNormalIncidence(0.075);
  clear_3mm.setBackSideSolarReflectanceatNormalIncidence(0.075);
  clear_3mm.setVisibleTransmittance(0.898);
  clear_3mm.setFrontSideVisibleReflectanceatNormalIncidence(0.081);
  clear_3mm.setBackSideVisibleReflectanceatNormalIncidence(0.081);
  clear_3mm.setInfraredTransmittance(0.0);
  clear_3mm.setFrontSideInfraredHemisphericalEmissivity(0.084);
  clear_3mm.setBackSideInfraredHemisphericalEmissivity(0.084);
  clear_3mm.setThermalConductivity(0.9);

  Gas air_13mm(model);
  air_13mm.setName("Air 13mm");
  air_13mm.setGasType("Air");
  air_13mm.setThickness(0.0127);

  //fenestrationMaterials.push_back(clear_3mm);
  //fenestrationMaterials.push_back(air_13mm);
  //fenestrationMaterials.push_back(clear_3mm);

  // DLM: use simple glazing so we can know window properties without requiring E+ run
  fenestrationMaterials.push_back(simple_glazing);

  Construction exteriorWindow(fenestrationMaterials);
  exteriorWindow.setName("Exterior Window");
  exteriorSubSurfaceConstructions.setFixedWindowConstruction(exteriorWindow);
  exteriorSubSurfaceConstructions.setOperableWindowConstruction(exteriorWindow);
  exteriorSubSurfaceConstructions.setGlassDoorConstruction(exteriorWindow);
  exteriorSubSurfaceConstructions.setSkylightConstruction(exteriorWindow);
  exteriorSubSurfaceConstructions.setTubularDaylightDomeConstruction(exteriorWindow);
  exteriorSubSurfaceConstructions.setTubularDaylightDiffuserConstruction(exteriorWindow);
  fenestrationMaterials.clear();

  // Exterior Door

  StandardOpaqueMaterial f08_metal_surface(model);
  f08_metal_surface.setName("F08 Metal surface");
  f08_metal_surface.setRoughness("Smooth");
  f08_metal_surface.setThickness(0.0008);
  f08_metal_surface.setThermalConductivity(45.28);
  f08_metal_surface.setDensity(7824.0);
  f08_metal_surface.setSpecificHeat(500.0);

  opaqueMaterials.push_back(f08_metal_surface);

  StandardOpaqueMaterial i01_25mm_insulation_board(model);
  i01_25mm_insulation_board.setName("I02 25mm insulation board");
  i01_25mm_insulation_board.setRoughness("MediumRough");
  i01_25mm_insulation_board.setThickness(0.0254);
  i01_25mm_insulation_board.setThermalConductivity(0.03);
  i01_25mm_insulation_board.setDensity(43.0);
  i01_25mm_insulation_board.setSpecificHeat(1210.0);

  opaqueMaterials.push_back(i01_25mm_insulation_board);

  Construction exteriorDoor(opaqueMaterials);
  exteriorDoor.setName("Exterior Door");
  exteriorDoor.setInsulation(i01_25mm_insulation_board);
  exteriorSubSurfaceConstructions.setDoorConstruction(exteriorDoor);
  exteriorSubSurfaceConstructions.setOverheadDoorConstruction(exteriorDoor);
  opaqueMaterials.clear();

  // Interior Window

  //fenestrationMaterials.push_back(clear_3mm);

  // DLM: use simple glazing so we can know window properties without requiring E+ run
  fenestrationMaterials.push_back(simple_glazing);

  Construction interiorWindow(fenestrationMaterials);
  interiorWindow.setName("Interior Window");
  interiorSubSurfaceConstructions.setFixedWindowConstruction(interiorWindow);
  interiorSubSurfaceConstructions.setOperableWindowConstruction(interiorWindow);
  interiorSubSurfaceConstructions.setGlassDoorConstruction(interiorWindow);
  interiorSubSurfaceConstructions.setSkylightConstruction(interiorWindow);
  interiorSubSurfaceConstructions.setTubularDaylightDomeConstruction(interiorWindow);
  interiorSubSurfaceConstructions.setTubularDaylightDiffuserConstruction(interiorWindow);
  fenestrationMaterials.clear();

  // Interior Door

  StandardOpaqueMaterial g05_25mm_wood(model);
  g05_25mm_wood.setName("G05 25mm wood");
  g05_25mm_wood.setRoughness("MediumSmooth");
  g05_25mm_wood.setThickness(0.0254);
  g05_25mm_wood.setThermalConductivity(0.15);
  g05_25mm_wood.setDensity(608.0);
  g05_25mm_wood.setSpecificHeat(1630.0);

  opaqueMaterials.push_back(g05_25mm_wood);

  Construction interiorDoor(opaqueMaterials);
  interiorDoor.setName("Interior Door");
  interiorSubSurfaceConstructions.setDoorConstruction(interiorDoor);
  interiorSubSurfaceConstructions.setOverheadDoorConstruction(interiorDoor);
  opaqueMaterials.clear();

  // Interior Partition

  opaqueMaterials.push_back(g05_25mm_wood);

  Construction interiorPartition(opaqueMaterials);
  interiorPartition.setName("Interior Partition");
  defaultConstructions.setInteriorPartitionConstruction(interiorPartition);
  opaqueMaterials.clear();
}

void usage( boost::program_options::options_description desc)
{
  std::cout << "Usage: builddemomodel --outputPath=./path/to/output.osm" << std::endl;
  std::cout << "   or: builddemomodel output.osm" << std::endl;
  std::cout << desc << std::endl;
}

int main(int argc, char *argv[])
{
  std::string outputPathString;

  boost::program_options::options_description desc("Allowed options");
  desc.add_options()
    ("help", "print help message")
    ("outputPath", boost::program_options::value<std::string>(&outputPathString), "OSM file to write out");
  boost::program_options::positional_options_description pos;
  pos.add("outputPath", -1);

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
  if(!vm.count("outputPath")) {
    std::cerr << "No output path given." << std::endl << std::endl;
    usage(desc);
    return EXIT_FAILURE;
  }

  openstudio::path outputPath = openstudio::toPath(outputPathString);
  
  return EXIT_SUCCESS;
}

