#ifndef PRO_VIDEO_SERVER_VERSION
#define PRO_VIDEO_SERVER_VERSION
extern "C"
{
char* EnergySaverVersionString(); 
int EnergySaverVersionMajor();
int EnergySaverVersionMinor();
int EnergySaverVersionType();
int EnergySaverVersionBuild();
}
#endif