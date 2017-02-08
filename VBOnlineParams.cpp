#include "VBOnlineParams.h"

VBOnlineParams:: VBOnlineParams(int pCurrPage,int pValid, int pRepeating, int pWrong, int pFailed)
{
    sCurrPage = pCurrPage;
    sValid = pValid;
    sRepeating = pRepeating;
    sWrong = pWrong;
    sFailed = pFailed;
}
