#include <stddef.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <AppKit/AppKit.h>

/* denotes UTF-8 char */
typedef char nfdchar_t;

/* opaque data structure -- see NFD_PathSet_* */
typedef struct {
    nfdchar_t *buf;
    size_t *indices; /* byte offsets into buf */
    size_t count;    /* number of indices into buf */
}nfdpathset_t;

typedef enum {
    NFD_ERROR,       /* programmatic error */
    NFD_OKAY,        /* user pressed okay, or successful return */
    NFD_CANCEL       /* user pressed cancel */
}nfdresult_t;
    

/* nfd_<targetplatform>.c */

/* single file open dialog */    
nfdresult_t NFD_OpenDialog( const nfdchar_t *filterList,
                            const nfdchar_t *defaultPath,
                            nfdchar_t **outPath );

/* multiple file open dialog */    
nfdresult_t NFD_OpenDialogMultiple( const nfdchar_t *filterList,
                                    const nfdchar_t *defaultPath,
                                    nfdpathset_t *outPaths );

/* save dialog */
nfdresult_t NFD_SaveDialog( const nfdchar_t *filterList,
                            const nfdchar_t *defaultPath,
                            nfdchar_t **outPath );


/* select folder dialog */
nfdresult_t NFD_PickFolder( const nfdchar_t *defaultPath,
                            nfdchar_t **outPath);

#define NFD_MAX_STRLEN 256
#define _NFD_UNUSED(x) ((void)x)

#define NFD_UTF8_BOM "\xEF\xBB\xBF"


void  *NFDi_Malloc( size_t bytes );
void   NFDi_Free( void *ptr );
void   NFDi_SetError( const char *msg );
int    NFDi_SafeStrncpy( char *dst, const char *src, size_t maxCopy );
int32_t NFDi_UTF8_Strlen( const nfdchar_t *str );
int    NFDi_IsFilterSegmentChar( char ch );

/* nfd_common.c */

/* get last error -- set when nfdresult_t returns NFD_ERROR */
const char *NFD_GetError( void );
/* get the number of entries stored in pathSet */
size_t      NFD_PathSet_GetCount( const nfdpathset_t *pathSet );
/* Get the UTF-8 path at offset index */
nfdchar_t  *NFD_PathSet_GetPath( const nfdpathset_t *pathSet, size_t index );
/* Free the pathSet */    
void        NFD_PathSet_Free( nfdpathset_t *pathSet );

static char g_errorstr[NFD_MAX_STRLEN] = {0};

/* public routines */

const char *NFD_GetError( void )
{
    return g_errorstr;
}

size_t NFD_PathSet_GetCount( const nfdpathset_t *pathset )
{
    assert(pathset);
    return pathset->count;
}

nfdchar_t *NFD_PathSet_GetPath( const nfdpathset_t *pathset, size_t num )
{
    assert(pathset);
    assert(num < pathset->count);
    
    return pathset->buf + pathset->indices[num];
}

void NFD_PathSet_Free( nfdpathset_t *pathset )
{
    assert(pathset);
    NFDi_Free( pathset->indices );
    NFDi_Free( pathset->buf );
}

/* internal routines */

void *NFDi_Malloc( size_t bytes )
{
    void *ptr = malloc(bytes);
    if ( !ptr )
        NFDi_SetError("NFDi_Malloc failed.");

    return ptr;
}

void NFDi_Free( void *ptr )
{
    assert(ptr);
    free(ptr);
}

void NFDi_SetError( const char *msg )
{
    int bTruncate = NFDi_SafeStrncpy( g_errorstr, msg, NFD_MAX_STRLEN );
    assert( !bTruncate );  _NFD_UNUSED(bTruncate);
}


int NFDi_SafeStrncpy( char *dst, const char *src, size_t maxCopy )
{
    size_t n = maxCopy;
    char *d = dst;

    assert( src );
    assert( dst );
    
    while ( n > 0 && *src != '\0' )    
    {
        *d++ = *src++;
        --n;
    }

    /* Truncation case -
       terminate string and return true */
    if ( n == 0 )
    {
        dst[maxCopy-1] = '\0';
        return 1;
    }

    /* No truncation.  Append a single NULL and return. */
    *d = '\0';
    return 0;
}


/* adapted from microutf8 */
int32_t NFDi_UTF8_Strlen( const nfdchar_t *str )
{
	/* This function doesn't properly check validity of UTF-8 character 
	sequence, it is supposed to use only with valid UTF-8 strings. */
    
	int32_t character_count = 0;
	int32_t i = 0; /* Counter used to iterate over string. */
	nfdchar_t maybe_bom[4];
	
	/* If there is UTF-8 BOM ignore it. */
	if (strlen(str) > 2)
	{
		strncpy(maybe_bom, str, 3);
		maybe_bom[3] = 0;
		if (strcmp(maybe_bom, (nfdchar_t*)NFD_UTF8_BOM) == 0)
			i += 3;
	}
	
	while(str[i])
	{
		if (str[i] >> 7 == 0)
        {
            /* If bit pattern begins with 0 we have ascii character. */ 
			++character_count;
        }
		else if (str[i] >> 6 == 3)
        {
		/* If bit pattern begins with 11 it is beginning of UTF-8 byte sequence. */
			++character_count;
        }
		else if (str[i] >> 6 == 2)
			;		/* If bit pattern begins with 10 it is middle of utf-8 byte sequence. */
		else
        {
            /* In any other case this is not valid UTF-8. */
			return -1;
        }
		++i;
	}

	return character_count;	
}

int NFDi_IsFilterSegmentChar( char ch )
{
    return (ch==','||ch==';'||ch=='\0');
}

static NSArray *BuildAllowedFileTypes( const char *filterList )
{
    // Commas and semicolons are the same thing on this platform

    NSMutableArray *buildFilterList = [[NSMutableArray alloc] init];

    char typebuf[NFD_MAX_STRLEN] = {0};
    
    size_t filterListLen = strlen(filterList);
    char *p_typebuf = typebuf;
    for ( size_t i = 0; i < filterListLen+1; ++i )
    {
        if ( filterList[i] == ',' || filterList[i] == ';' || filterList[i] == '\0' )
        {
            if (filterList[i] != '\0')
                ++p_typebuf;
            *p_typebuf = '\0';

            NSString *thisType = [NSString stringWithUTF8String: typebuf];
            [buildFilterList addObject:thisType];
            p_typebuf = typebuf;
            *p_typebuf = '\0';
        }
        else
        {
            *p_typebuf = filterList[i];
            ++p_typebuf;

        }
    }

    NSArray *returnArray = [NSArray arrayWithArray:buildFilterList];

    [buildFilterList release];
    return returnArray;
}

static void AddFilterListToDialog( NSSavePanel *dialog, const char *filterList )
{
    if ( !filterList || strlen(filterList) == 0 )
        return;

    NSArray *allowedFileTypes = BuildAllowedFileTypes( filterList );
    if ( [allowedFileTypes count] != 0 )
    {
        [dialog setAllowedFileTypes:allowedFileTypes];
    }
}

static void SetDefaultPath( NSSavePanel *dialog, const nfdchar_t *defaultPath )
{
    if ( !defaultPath || strlen(defaultPath) == 0 )
        return;

    NSString *defaultPathString = [NSString stringWithUTF8String: defaultPath];
    NSURL *url = [NSURL fileURLWithPath:defaultPathString isDirectory:YES];
    [dialog setDirectoryURL:url];    
}


/* fixme: pathset should be pathSet */
static nfdresult_t AllocPathSet( NSArray *urls, nfdpathset_t *pathset )
{
    assert(pathset);
    assert([urls count]);

    pathset->count = (size_t)[urls count];
    pathset->indices = NFDi_Malloc( sizeof(size_t)*pathset->count );
    if ( !pathset->indices ) 
    {
        return NFD_ERROR;
    }

    // count the total space needed for buf
    size_t bufsize = 0;
    for ( NSURL *url in urls )
    {
        NSString *path = [url path];
        bufsize += [path lengthOfBytesUsingEncoding:NSUTF8StringEncoding] + 1;
    }

    pathset->buf = NFDi_Malloc( sizeof(nfdchar_t) * bufsize );
    if ( !pathset->buf )
    {
        return NFD_ERROR;
    }

    // fill buf
    nfdchar_t *p_buf = pathset->buf;
    size_t count = 0;
    for ( NSURL *url in urls )
    {
        NSString *path = [url path];
        const nfdchar_t *utf8Path = [path UTF8String];
        size_t byteLen = [path lengthOfBytesUsingEncoding:NSUTF8StringEncoding] + 1;
        memcpy( p_buf, utf8Path, byteLen );

        ptrdiff_t index = p_buf - pathset->buf;
        assert( index >= 0 );
        pathset->indices[count] = (size_t)index;

        p_buf += byteLen;
        ++count;
    }

    return NFD_OKAY;
}

/* public */


nfdresult_t NFD_OpenDialog( const nfdchar_t *filterList,
                            const nfdchar_t *defaultPath,
                            nfdchar_t **outPath )
{
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];

    NSWindow *keyWindow = [[NSApplication sharedApplication] keyWindow];    
    NSOpenPanel *dialog = [NSOpenPanel openPanel];
    [dialog setAllowsMultipleSelection:NO];

    // Build the filter list
    AddFilterListToDialog(dialog, filterList);

    // Set the starting directory
    SetDefaultPath(dialog, defaultPath);

    nfdresult_t nfdResult = NFD_CANCEL;
    if ( [dialog runModal] == NSModalResponseOK )
    {
        NSURL *url = [dialog URL];
        const char *utf8Path = [[url path] UTF8String];

        // byte count, not char count
        size_t len = strlen(utf8Path);//NFDi_UTF8_Strlen(utf8Path);

        *outPath = NFDi_Malloc( len+1 );
        if ( !*outPath )
        {
            [pool release];
            [keyWindow makeKeyAndOrderFront:nil];            
            return NFD_ERROR;
        }
        memcpy( *outPath, utf8Path, len+1 ); /* copy null term */
        nfdResult = NFD_OKAY;
    }
    [pool release];

    [keyWindow makeKeyAndOrderFront:nil];
    return nfdResult;
}


nfdresult_t NFD_OpenDialogMultiple( const nfdchar_t *filterList,
                                    const nfdchar_t *defaultPath,
                                    nfdpathset_t *outPaths )
{
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
    NSWindow *keyWindow = [[NSApplication sharedApplication] keyWindow];
    
    NSOpenPanel *dialog = [NSOpenPanel openPanel];
    [dialog setAllowsMultipleSelection:YES];

    // Build the fiter list.
    AddFilterListToDialog(dialog, filterList);

    // Set the starting directory
    SetDefaultPath(dialog, defaultPath);
    
    nfdresult_t nfdResult = NFD_CANCEL;
    if ( [dialog runModal] == NSModalResponseOK )
    {
        NSArray *urls = [dialog URLs];

        if ( [urls count] == 0 )
        {
            [pool release];
            [keyWindow makeKeyAndOrderFront:nil];            
            return NFD_CANCEL;
        }

        if ( AllocPathSet( urls, outPaths ) == NFD_ERROR )
        {
            [pool release];
            [keyWindow makeKeyAndOrderFront:nil];            
            return NFD_ERROR;
        }

        nfdResult = NFD_OKAY;
    }
    [pool release];

    [keyWindow makeKeyAndOrderFront:nil];    
    return nfdResult;
}


nfdresult_t NFD_SaveDialog( const nfdchar_t *filterList,
                            const nfdchar_t *defaultPath,
                            nfdchar_t **outPath )
{
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
    NSWindow *keyWindow = [[NSApplication sharedApplication] keyWindow];
    
    NSSavePanel *dialog = [NSSavePanel savePanel];
    [dialog setExtensionHidden:NO];
    
    // Build the filter list.
    AddFilterListToDialog(dialog, filterList);

    // Set the starting directory
    SetDefaultPath(dialog, defaultPath);

    nfdresult_t nfdResult = NFD_CANCEL;
    if ( [dialog runModal] == NSModalResponseOK )
    {
        NSURL *url = [dialog URL];
        const char *utf8Path = [[url path] UTF8String];

        size_t byteLen = [url.path lengthOfBytesUsingEncoding:NSUTF8StringEncoding] + 1;
        
        *outPath = NFDi_Malloc( byteLen );
        if ( !*outPath )
        {
            [pool release];
            [keyWindow makeKeyAndOrderFront:nil];            
            return NFD_ERROR;
        }
        memcpy( *outPath, utf8Path, byteLen );
        nfdResult = NFD_OKAY;
    }

    [pool release];
    [keyWindow makeKeyAndOrderFront:nil];
    return nfdResult;
}

nfdresult_t NFD_PickFolder(const nfdchar_t *defaultPath,
    nfdchar_t **outPath)
{
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];

    NSWindow *keyWindow = [[NSApplication sharedApplication] keyWindow];
    NSOpenPanel *dialog = [NSOpenPanel openPanel];
    [dialog setAllowsMultipleSelection:NO];
    [dialog setCanChooseDirectories:YES];
    [dialog setCanCreateDirectories:YES];
    [dialog setCanChooseFiles:NO];

    // Set the starting directory
    SetDefaultPath(dialog, defaultPath);

    nfdresult_t nfdResult = NFD_CANCEL;
    if ( [dialog runModal] == NSModalResponseOK )
    {
        NSURL *url = [dialog URL];
        const char *utf8Path = [[url path] UTF8String];

        // byte count, not char count
        size_t len = strlen(utf8Path);//NFDi_UTF8_Strlen(utf8Path);

        *outPath = NFDi_Malloc( len+1 );
        if ( !*outPath )
        {
            [pool release];
            [keyWindow makeKeyAndOrderFront:nil];            
            return NFD_ERROR;
        }
        memcpy( *outPath, utf8Path, len+1 ); /* copy null term */
        nfdResult = NFD_OKAY;
    }
    [pool release];

    [keyWindow makeKeyAndOrderFront:nil];
    return nfdResult;
}
