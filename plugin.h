/*********************************************************************
 * This is a part of Angry IP Scanner source code                    *
 * http://www.angryziber.com/ipscan/                                 *
 *                                                                   *
 * Written by Angryziber                                             *
 *                                                                   *
 * You may distribute this code as long as this message is not       *
 * removed and it is clear who has written it.                       *
 * You may not rename the program and distribute it.                 *
 *********************************************************************/

// plugin.h: structures and definitions for Angry IP Scanner plugins
//
//////////////////////////////////////////////////////////////////////

// Possible plugin types (specify these in nPluginType field of TInfoStruct)

#define PLUGIN_TYPE_COLUMN       0		// New List Column (for scanning)
#define PLUGIN_TYPE_OUTPUT       1		// New output format (currently not supported)
#define PLUGIN_TYPE_IP_RANGE     2		// New possibility to specify IPs for scanning (currently not supported)

///////////////////////////////////////////////////////////////////////////////////////////
// Structure that is filled by Info plugin function
///////////////////////////////////////////////////////////////////////////////////////////

typedef struct
{	
	int nStructSize;                        // Size of this structure in bytes, this is preset by Angry IP Scanner
	int nAngryIPScannerVersion;             // Known supported version of Angry IP Scanner, eg 217 (instead of 2.17)
	int nPluginType;                        // Type of the plugin, see PLUGIN_TYPE_* constants
	char szPluginName[32];                  // Column name in the list (plugin idenificator)
	char szDescription[1024];               // Description of plugin
	char szAuthorName[32];                  // Author's name
	char szPluginWebsite[96];               // URL of plugin on the web
	char szAuthorEmail[64];                 // Author's email
	char bMultipleScanningSupported;        // Specifies whether this plugin can be used as multiple columns in the list, 1 or 0.
	char szReserved[128];                   // Reserved bytes for future additions. Do not change them.
} 
TInfoStruct;

//////////////////////////////////////////////////////////////////////////////////////////
// These functions may/must be exported from plugin DLLs.
//////////////////////////////////////////////////////////////////////////////////////////

// Common functions for all types of plugins
//////////////////////////////////////////////////

// "Info" function.
// This function is required for all types of plugins
// After loading a plugin into memory, Angry IP Scanner runs this function
// in order to check that it is a valid plugin and that it is written for
// the correct version of Angry IP Scanner. 
// Parameters:
//   pInfoStruct - is used for getting information about the plugin
//                 plugins must fill this structure with correct data
//                 before exiting this function. 
typedef BOOL (__cdecl TInfoFunction)(TInfoStruct *pInfoStruct);

// Optional functions (for all types of plugins)
// If they exist, they will be called
///////////////////////////////////////////////////

// "Options" function.
// This function is executed when user wants to change settings of the plugin.
// User interface must be provided by this function, it is achievable by displaying
// a dialog box. Note: it is important to save settings according to the index of
// the plugin, see below the description of Init() function.
// Parameters:
//    hwndParent - window handle of parent window in case dialog box must be displayed.
typedef BOOL (__cdecl TOptionsFunction)(HWND hwndParent);

// "Init" function.
// This function is executed before scanning process begins. It can be used for internal
// initializations, such as allocating of memory, loading of settings, etc.
// Parameters:
//    nIndex - this is the unique index of this plugin. In Angry IP Scanner, plugins
//             can be selected multiple times to represent multiple columns in the list.
//             Most likely these multiple columns of a single plugin will need different settings,
//             so, these settings must be stored separately, eg this index can be added to the
//             end of each key name in registry to make it unique: "PingTimeout2" instead 
//             of plain "PingTimeout".
typedef BOOL (__cdecl TInitFunction)(int nIndex);

// "Finalize" function.
// This function is executed after the scanning process has been finished. It can be used
// for deallocation of previously allocated memory, etc.
// Parameters:
//   None
typedef BOOL (__cdecl TFinalizeFunction)();

// Functions for PLUGIN_TYPE_COLUMN
/////////////////////////////////////////////////////

// "Scan" function.
// This is the main function to do the scanning. This function receves the IP
// address to scan, does it's job and returns output as a text string, which
// is then displayed on the list. This function must support multithreading,
// because it will be called many times in parallel to increase scanning 
// speed.
// Parameters:
//   nIP - the IP address to gather information about
//   szReturn - the buffer for returning scanning results to.
//   nBufferLen - length of the passed buffer in bytes.
typedef BOOL (__cdecl TScanFunction)(DWORD nIP, LPSTR szReturn, int nBufferLen);


// The end!