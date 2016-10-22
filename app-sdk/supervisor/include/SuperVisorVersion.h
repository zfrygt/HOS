#ifndef PRO_VIDEO_SERVER_VERSION
#define PRO_VIDEO_SERVER_VERSION
extern "C"
{
char* proVideoServerVersionString(); 
int proVideoServerVersionMajor();
int proVideoServerVersionMinor();
int proVideoServerVersionType();
int proVideoServerVersionBuild();
}
#endif