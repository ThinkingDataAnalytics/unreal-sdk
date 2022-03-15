// Copyright 2021 ThinkingData. All Rights Reserved.
#pragma once

class FTAConstants
{
public:
	
	constexpr static char const* const KEY_TYPE = "#type";
	constexpr static char const* const KEY_EVENT_NAME = "#event_name";
	constexpr static char const* const KEY_SYSTEM_LANGUAGE = "#system_language";
   	constexpr static char const* const KEY_APP_ID = "#app_id";
   	constexpr static char const* const KEY_TIME = "#time";
   	constexpr static char const* const KEY_FLUSH_TIME = "#flush_time";
   	constexpr static char const* const KEY_DATA = "data";
   	constexpr static char const* const KEY_DATA_ID = "#uuid";
   	constexpr static char const* const KEY_ACCOUNT_ID = "#account_id";
   	constexpr static char const* const KEY_DISTINCT_ID = "#distinct_id";
   	constexpr static char const* const KEY_CPU = "#cpu";
   	constexpr static char const* const KEY_GPU = "#gpu";
   	constexpr static char const* const KEY_NETWORK_TYPE = "#network_type";
   	constexpr static char const* const KEY_DEVICE_ID = "#device_id";
   	constexpr static char const* const KEY_SCREEN_HEIGHT = "#screen_height";
   	constexpr static char const* const KEY_SCREEN_WIDTH = "#screen_width";
   	constexpr static char const* const KEY_OS = "#os";
   	constexpr static char const* const KEY_OS_VERSION = "#os_version";
   	constexpr static char const* const KEY_APP_VERSION = "#app_version";
   	constexpr static char const* const KEY_LIB_VERSION = "#lib_version";
   	constexpr static char const* const KEY_ZONE_OFFSET = "#zone_offset";
	constexpr static char const* const KEY_PROPERTIES = "properties";
   	const static uint32 USER_INDEX = 67;

	//EVENT TYPE
   	constexpr static char const* const EVENTTYPE_TRACK = "track";
   	constexpr static char const* const EVENTTYPE_TRACK_UPDATE = "track_update";
   	constexpr static char const* const EVENTTYPE_TRACK_OVERWRITE = "track_overwrite";
   	constexpr static char const* const EVENTTYPE_TRACK_FIRST = "track_first";
   	constexpr static char const* const EVENTTYPE_USER_ADD = "user_add";
   	constexpr static char const* const EVENTTYPE_USER_SET = "user_set";
   	constexpr static char const* const EVENTTYPE_USER_SET_ONCE = "user_setOnce";
   	constexpr static char const* const EVENTTYPE_USER_UNSET = "user_unset";
   	constexpr static char const* const EVENTTYPE_USER_APPEND = "user_append";
   	constexpr static char const* const EVENTTYPE_USER_DEL = "user_del";
   	constexpr static char const* const KEY_EVENT_ID  = "#event_id";
   	constexpr static char const* const KEY_FIRST_CHECK_ID = "#first_check_id";

	//AUTO TRACK EVENT TYPE
	constexpr static char const* const AUTOTRACK_EVENTTYPE_INSTALL = "ta_app_install";
	constexpr static char const* const AUTOTRACK_EVENTTYPE_START = "ta_app_startl";
	constexpr static char const* const AUTOTRACK_EVENTTYPE_END = "ta_app_end";
	constexpr static char const* const AUTOTRACK_EVENTTYPE_CLICK = "ta_app_click";
	constexpr static char const* const AUTOTRACK_EVENTTYPE_CRASH = "ta_app_crash";
	constexpr static char const* const AUTOTRACK_EVENTTYPE_VIEW_SCREEN = "ta_app_view";
};