/*

 Copyright (c) 2013, Ford Motor Company
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:

 Redistributions of source code must retain the above copyright notice, this
 list of conditions and the following disclaimer.

 Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following
 disclaimer in the documentation and/or other materials provided with the
 distribution.

 Neither the name of the Ford Motor Company nor the names of its contributors
 may be used to endorse or promote products derived from this software
 without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 POSSIBILITY OF SUCH DAMAGE.
 */

#include "application_manager/commands/mobile/unsubscribe_vehicle_data_request.h"
#include "application_manager/application_manager_impl.h"
#include "application_manager/application_impl.h"
#include "application_manager/message_helper.h"
#include "interfaces/MOBILE_API.h"
#include "smart_objects/smart_object.h"

namespace application_manager {

namespace commands {

UnsubscribeVehicleDataRequest::UnsubscribeVehicleDataRequest(
    const MessageSharedPtr& message)
    : CommandRequestImpl(message) {
}

UnsubscribeVehicleDataRequest::~UnsubscribeVehicleDataRequest() {
}

void UnsubscribeVehicleDataRequest::Run() {
  LOG4CXX_INFO(logger_, "UnsubscribeVehicleDataRequest::Run");

  Application* app = ApplicationManagerImpl::instance()->application(
      CommandRequestImpl::connection_key()
      );

  if (NULL == app) {
    LOG4CXX_ERROR(logger_, "NULL pointer");
    SendResponse(false, mobile_apis::Result::APPLICATION_NOT_REGISTERED);
    return;
  }

  // counter for items to subscribe
  int items_to_unsubscribe = 0;
  // counter for subscribed items by application
  int unsubscribed_items = 0;

  const VehicleData& vehicle_data = MessageHelper::vehicle_data();
  VehicleData::const_iterator it = vehicle_data.begin();

  smart_objects::SmartObject msg_params = smart_objects::SmartObject(
      smart_objects::SmartType_Map);

  msg_params[strings::app_id] = app->app_id();

  for (; vehicle_data.end() != it; ++it) {
    std::string key_name = it->first;
    if ((*message_)[strings::msg_params].keyExists(key_name)) {
      bool is_key_enabled = (*message_)[strings::msg_params][key_name].asBool();
      if (is_key_enabled) {
        ++items_to_unsubscribe;
        msg_params[key_name] = is_key_enabled;
      }

      VehicleDataType key_type = it->second;
      if (app->UnsubscribeFromIVI(static_cast<unsigned int>(key_type))) {
        ++unsubscribed_items;
      }
    }
  }

  if (0 == items_to_unsubscribe) {
    SendResponse(false,
                 mobile_apis::Result::INVALID_DATA,
                 "No data in the request", &msg_params);
    return;
  } else if (0 == unsubscribed_items) {
    SendResponse(false,
                 mobile_apis::Result::IGNORED,
                 "Was not subscribed on any VehicleData", &msg_params);
    return;
  }

  SendHMIRequest(hmi_apis::FunctionID::VehicleInfo_UnsubscribeVehicleData,
                 &msg_params,
                 true);
}

void UnsubscribeVehicleDataRequest::on_event(const event_engine::Event& event){
  LOG4CXX_INFO(logger_, "UnsubscribeVehicleDataRequest::on_event");

  const smart_objects::SmartObject& message = event.smart_object();

  hmi_apis::Common_Result::eType hmi_result =
      static_cast<hmi_apis::Common_Result::eType>(
          message[strings::params][hmi_response::code].asInt()
          );

  bool result =
      hmi_result == hmi_apis::Common_Result::SUCCESS;

  mobile_apis::Result::eType result_code =
      hmi_result == hmi_apis::Common_Result::SUCCESS
      ? mobile_apis::Result::SUCCESS
      : static_cast<mobile_apis::Result::eType>(
          message[strings::params][hmi_response::code].asInt()
          );

  const char* return_info = NULL;

  if (result) {
    if (IsAnythingAlreadyUnsubscribed()) {
      result_code = mobile_apis::Result::WARNINGS;
      return_info = std::string("Unsupported phoneme type sent in a prompt").c_str();
    }
  }


  SendResponse(result,
               result_code,
               return_info,
               &(message[strings::msg_params]));
}

bool SubscribeVehicleDataRequest::IsAnythingAlreadyUnsubscribed() {
  LOG4CXX_INFO(logger_, "SubscribeVehicleDataRequest::Run");

  const VehicleData& vehicle_data = MessageHelper::vehicle_data();
  VehicleData::const_iterator it = vehicle_data.begin();

  for (; vehicle_data.end() != it; ++it) {
    if (true == (*message_)[strings::msg_params].keyExists(it->first)) {

      if ((*message_)[strings::msg_params][it->first]
          [strings::result_code].asInt() ==
          hmi_apis::Common_VehicleDataResultCode::VDRC_DATA_NOT_SUBSCRIBED) {
        return true;
      }
    }
  }

  return false;
}


}  // namespace commands

}  // namespace application_manager
