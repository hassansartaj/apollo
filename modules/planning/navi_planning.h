/******************************************************************************
 * Copyright 2018 The Apollo Authors. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *****************************************************************************/

#ifndef MODULES_PLANNING_NAVI_PLANNING_H_
#define MODULES_PLANNING_NAVI_PLANNING_H_

#include <string>
#include <utility>
#include <vector>

#include "modules/planning/planning_base.h"
#include "modules/planning/proto/pad_msg.pb.h"

/**
 * @namespace apollo::planning
 * @brief apollo::planning
 */
namespace apollo {
namespace planning {

/**
 * @class planning
 *
 * @brief Planning module main class. It processes GPS and IMU as input,
 * to generate planning info.
 */
class NaviPlanning : public PlanningBase {
 public:
  NaviPlanning() = default;
  virtual ~NaviPlanning();

  /**
   * @brief Planning algorithm name.
   */
  std::string Name() const override;

  /**
   * @brief module initialization function
   * @return initialization status
   */
  apollo::common::Status Init() override;

  /**
   * @brief module start function
   * @return start status
   */
  apollo::common::Status Start() override;

  /**
   * @brief module stop function
   */
  void Stop() override;

  /**
   * @brief main logic of the planning module, runs periodically triggered by
   * timer.
   */
  void RunOnce() override;

  void OnTimer(const ros::TimerEvent&) override;

 private:
  /**
   * @brief receiving planning pad message
   */
  void OnPad(const PadMessage& pad);

  /**
   * @brief make driving decisions by received planning pad msg
   */
  void ProcessPadMsg(DrivingAction drvie_action);

  /**
   * @brief get the lane Id of the lane in which the vehicle is located
   */
  std::string GetCurrentLaneId();

  /**
   * @brief get the left neighbors lane info of the lane which the vehicle is
   *located
   * @lane_info_group output left neighors info which sorted from near to
   *far
   */
  void GetLeftNeighborLanesInfo(
      std::vector<std::pair<std::string, double>>* const lane_info_group);

  /**
   * @brief get the right neighbors lane of the lane which the vehicle is
   * located
   * @lane_info_group output right neighors info which sorted from near to
   *far
   */
  void GetRightNeighborLanesInfo(
      std::vector<std::pair<std::string, double>>* const lane_info_group);

  void SetFallbackTrajectory(ADCTrajectory* cruise_trajectory) override;

  class VehicleConfig {
   public:
    double x_ = 0.0;
    double y_ = 0.0;
    double theta_ = 0.0;
    bool is_valid_ = false;
  };
  VehicleConfig last_vehicle_config_;

  VehicleConfig ComputeVehicleConfigFromLocalization(
      const localization::LocalizationEstimate& localization) const;

  std::string target_lane_id_;
  DrivingAction driving_action_;
  bool is_received_pad_msg_ = false;
};

}  // namespace planning
}  // namespace apollo

#endif  // MODULES_PLANNING_NAVI_PLANNING_H_