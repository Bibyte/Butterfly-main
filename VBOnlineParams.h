#ifndef VBONLINEPARAMS_H
#define VBONLINEPARAMS_H

class VBOnlineParams
{
public:
    int sCurrPage,sValid, sRepeating, sWrong, sFailed;
    VBOnlineParams(int pCurrPage,int pValid, int pRepeating, int pWrong, int pFailed);
};

#endif // VBONLINEPARAMS_H
