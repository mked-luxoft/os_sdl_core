/*
 * Copyright (c) 2016, Ford Motor Company
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following
 * disclaimer in the documentation and/or other materials provided with the
 * distribution.
 *
 * Neither the name of the Ford Motor Company nor the names of its contributors
 * may be used to endorse or promote products derived from this software
 * without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_TEST_INCLUDE_APPLICATION_MANAGER_MOCK_RESUME_CTRL_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_TEST_INCLUDE_APPLICATION_MANAGER_MOCK_RESUME_CTRL_H_
#include "gmock/gmock.h"
#include "application_manager/resumption/resume_ctrl.h"
#include "application_manager/application.h"
#include "resumption/last_state.h"

namespace resumption {

class MockResumeCtrl : public ResumeCtrl {
 public:
  MOCK_METHOD1(SaveApplication,
               void(app_mngr::ApplicationSharedPtr application));
  MOCK_METHOD1(RemoveApplicationFromSaved,
               bool(app_mngr::ApplicationConstSharedPtr application));
  MOCK_METHOD0(OnSuspend, void());
  MOCK_METHOD0(OnAwake, void());
  MOCK_METHOD0(StopSavePersistentDataTimer, void());
  MOCK_METHOD2(StartResumption,
               bool(app_mngr::ApplicationSharedPtr application,
                    const std::string& hash));
  MOCK_METHOD1(StartResumptionOnlyHMILevel,
               bool(app_mngr::ApplicationSharedPtr application));
  MOCK_METHOD1(CheckPersistenceFilesForResumption,
               bool(app_mngr::ApplicationSharedPtr application));
  MOCK_METHOD2(CheckApplicationHash,
               bool(app_mngr::ApplicationSharedPtr application,
                    const std::string& hash));
  MOCK_METHOD1(IsHMIApplicationIdExist, bool(uint32_t hmi_app_id));
  MOCK_METHOD2(IsApplicationSaved,
               bool(const std::string& policy_app_id,
                    const std::string& device_id));
  MOCK_CONST_METHOD2(GetHMIApplicationID,
                     uint32_t(const std::string& policy_app_id,
                              const std::string& device_id));
  MOCK_METHOD0(ApplicationsDataUpdated, void());
  MOCK_METHOD0(ResetLaunchTime, void());
  MOCK_METHOD1(OnAppActivated,
               void(app_mngr::ApplicationSharedPtr application));
  MOCK_METHOD1(RemoveFromResumption, void(uint32_t app_id));
  MOCK_METHOD1(Init, bool(LastState& last_state));
  MOCK_METHOD2(OnAppRegistrationStart,
               void(const std::string& policy_app_id,
                    const std::string& device_id));
  MOCK_METHOD0(OnAppRegistrationEnd, void());
  MOCK_METHOD1(set_resumption_storage,
               void(utils::SharedPtr<ResumptionData> mock_storage));
};

}  // namespace resumption

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_TEST_INCLUDE_APPLICATION_MANAGER_MOCK_RESUME_CTRL_H_
