current_path=`pwd`
sdk_version_tag='VersionName'
sdk_version_file=$current_path/TAUnrealDemo/Plugins/ThinkingAnalytics/ThinkingAnalytics.uplugin
sdk_current_version=`cat $sdk_version_file | grep -Eo [0-9]+\.[0-9]+\.[0-9]`
SDK_VERSION=$sdk_current_version
mkdir -p release
pushd release
zip -r ta_unreal_sdk.zip  ../TAUnrealDemo/Plugins/ThinkingAnalytics
zip -r "ta_unreal_sdk_v${sdk_current_version}.zip" ../TAUnrealDemo/Plugins/ThinkingAnalytics
popd
