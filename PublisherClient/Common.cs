using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Collections.Concurrent;

namespace PublisherClient
{
    /// <summary>
    /// Enum identifies all possible commands which can be sent to server
    /// </summary>
    internal enum HttpMessageType
    {
        HMT_UPLOADDIR,
        HMT_UNDEFINED,
        HMT_UPLOADFILE,
        HMT_BROWSEDIR,
        HMT_DELETEFILE,
        HMT_PUBLISH,
        HMT_DEP,
        HMT_DELETEFOLDER,
        HMT_DOWNLOADFILE
    }
}
