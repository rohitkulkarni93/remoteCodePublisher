#pragma once

enum HttpMessageType
{
	HMT_UNDEFINED,			//	Undefined message type
	HMT_UPLOADFILE,			//	Sending a file.
	HMT_UPLOADDIR,
	HMT_BROWSEDIR,			//	Browsing a directory
	HMT_DELETEFILE,			//	Deleting a file
	HMT_PUBLISH,			//	Publish file to html
	HMT_DEP,				//	Run dep analysis
	HMT_DELETEFOLDER,		//	Delete folder form server
	HMT_DOWNLOADFILE
};