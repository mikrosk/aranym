#include "nf_objs.h"

#include "nf_name.h"
#include "nf_version.h"
#include "nf_stderr.h"
#include "xhdi.h"
#include "fvdidrv.h"
/* add your NatFeat class definition here */

NF_Name nf_name;
NF_Version nf_version;
NF_StdErr nf_stderr;
XHDIDriver Xhdi;
FVDIDriver fVDIDrv;
/* add your NatFeat object declaration here */

pNatFeat nf_objects[] = {
	&nf_name,
	&nf_version,
	&nf_stderr,
	&Xhdi,
	&fVDIDrv
	/* add your NatFeat object here */
};

unsigned int nf_objs_cnt = sizeof(nf_objects) / sizeof(nf_objects[0]);
