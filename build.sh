SDK_VERSION="1.3.0"

mkdir -p release
pushd release
zip -r ta_unreal_sdk.zip  ../ThinkingAnalytics
zip -r ta_unrealsdk_v${SDK_VERSION}.zip ../ThinkingAnalytics
popd
