#include "twitcrawl.h"
#include "api.h"

#define MAX_CONN    450 /* Max connections per session */
#define VERSION     "1.4"

/* Show the monkey how */
void usage(const std::string &nameStr)
{
    std::cout << "Version: " << VERSION << std::endl;
    std::cout << nameStr << ": -f [FILE]" << std::endl;
}

int main( int argc, char* argv[] )
{
    std::string keywordList;
    if (argc < 2)
    {
        usage(argv[0]);
        return 1;
    }
    for( int i = 1; i < argc; i += 2 )
    {
        if( !strncmp( argv[i], "-f", strlen("-f") ) )
        {
            keywordList = argv[i+1];
        }
        else if( !strncmp( argv[i], "-v", strlen("-v") ) )
        {
            std::cout << "Consumer key:\t\t" << CONSUMER_KEY << std::endl;
            std::cout << "Consumer secret:\t" << CONSUMER_SECRET << std::endl;

            std::cout << "Token:\t\t\t" << ACCESS_TOKEN << std::endl;
            std::cout << "Token secret:\t\t" << ACCESS_TOKEN_SECRET << std::endl;
            return 1;
        }
        else
        {
            usage(argv[0]);
            return 1;
        }
    }
    if( !keywordList.length() )
    {
        usage(argv[0]);
        return 1;
    }

    twitCurl twitterObj;
    std::string tmpStr, tmpStr2;
    std::string replyMsg;

    /* Lets get authenticated */
    twitterObj.getOAuth().setConsumerKey( std::string( CONSUMER_KEY ) );
    twitterObj.getOAuth().setConsumerSecret( std::string( CONSUMER_SECRET ) );

    twitterObj.getOAuth().setOAuthTokenKey( ACCESS_TOKEN );
    twitterObj.getOAuth().setOAuthTokenSecret( ACCESS_TOKEN_SECRET );

    /*twitterObj.accountVerifyCredGet();
    {
        twitterObj.getLastWebResponse( replyMsg );
        std::cerr << "error" << replyMsg << std::endl;
    }*/

#if 0
    /* Post a new status message */
    char tmpBuf[1024];
    memset( tmpBuf, 0, 1024 );
    printf( "\nEnter a new status message: " );
    gets( tmpBuf );
    tmpStr = tmpBuf;
    replyMsg = "";
    if( twitterObj.statusUpdate( tmpStr ) )
    {
        twitterObj.getLastWebResponse( replyMsg );
        printf( "\nresponse:\n%s\n", replyMsg.c_str() );
    }
    else
    {
        twitterObj.getLastCurlError( replyMsg );
        fprintf( stderr, "\nerror:\n%s\n", replyMsg.c_str() );
        exit(0);
    }
    return 1;
#endif

    /* Prepare this session */
    time_t now = time( 0 );
    std::stringstream ss;
    ss << now;
    std::string dirStr = ss.str();
    mkdir( dirStr.c_str(), 0777 );

    /* Loop over keyword file */
    std::ifstream keywordListIn( keywordList.c_str() );
    std::string lineStr;
    int n = 0;
    while ( std::getline( keywordListIn, lineStr ) )
    {
        if ( n >= MAX_CONN )
        {
            std::cerr << "Max keywords per session" << std::endl;
            break;
        }

        tmpStr = lineStr;
        replyMsg = "";
	std::cout << "Request " + lineStr << std::endl;
        if( twitterObj.search( tmpStr ) )
        {
            twitterObj.getLastWebResponse( replyMsg );

            tmpStr = dirStr + "/" + tmpStr + std::string(".json");
            std::ofstream replyMsgOut;

            replyMsgOut.open( tmpStr.c_str(), std::ofstream::out );
            replyMsgOut.clear();

            replyMsgOut << replyMsg.c_str();
            replyMsgOut.close();
        }
        else
        {
            twitterObj.getLastCurlError( replyMsg );
            fprintf( stderr, "\nerror:\n%s\n", replyMsg.c_str() );
        }

        n++;
    }

    return 0;
}
