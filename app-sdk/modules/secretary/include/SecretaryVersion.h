#ifndef PRO_VIDEO_SERVER_VERSION
#define PRO_VIDEO_SERVER_VERSION
extern "C"
{
char* SecretaryVersionString(); 
int SecretaryVersionMajor();
int SecretaryVersionMinor();
int SecretaryVersionType();
int SecretaryVersionBuild();
}
#endif