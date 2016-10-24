#ifndef PRO_VIDEO_SERVER_VERSION
#define PRO_VIDEO_SERVER_VERSION
extern "C"
{
char* ReceptionistVersionString(); 
int ReceptionistVersionMajor();
int ReceptionistVersionMinor();
int ReceptionistVersionType();
int ReceptionistVersionBuild();
}
#endif