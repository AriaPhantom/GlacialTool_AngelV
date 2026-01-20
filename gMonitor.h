#pragma once
class gMonitor
{
public:
    gMonitor()
    {
        currentPlayerCoords[0] = -1;
        currentPlayerCoords[1] = -1;

        runeCoords[0] = -1;
        runeCoords[1] = -1;

        mapBorderCoords[0] = 200;
        mapBorderCoords[1] = 300;

        whiteIconCoords[0] = 0;
        whiteIconCoords[1] = 6;

        randomPlayerInMap = 0;
        randomPlayerTimer = 0;
        randomPlayerSamePosTimer = 0;
        randomPlayerOldPosX = -1;
        randomPlayerOldPosY = -1;

        friendPlayerInMap = 0;
        friendPlayerTimer = -1;

        whiteTimer = -1;
        runeclick = 0;
        portalclick = 0;
        runecapture = 0;

        status = 1;
        detectionStatus = 1;
        whiteIconUpdate = 1;
    }

    void setPlayerCoords(int x, int y);
    int* getPlayerCoords();

    void setWhiteIconCoords(int x, int y);
    int* getWhiteIconCoords();

    void setWhiteIconUpdate(int x);

    void setPortalclick(int portalclick);
    void reducePortalclick();
    int getPortalclick();

    void setRunecapture(int runecapture);

    void setRuneclick(int runeclick);
    void reduceRuneclick();
    int getRuneclick();

    void setRuneCoords(int x, int y);
    int* getRuneCoords();

    void setRandomPlayerInMap(int randomPlayerInMap);
    int isRandomPlayerInMap();
    void setRandomPlayerTimer(long val);
    long getRandomPlayerTimer();

    void setFriendPlayerInMap(int friendPlayerInMap);
    int isFriendPlayerInMap();
    void setFriendPlayerTimer(long val);
    long getFriendPlayerTimer();

    void setWhiteTimer(long val) {
        whiteTimer = val;
    }

    long getWhiteTimer() {
        return whiteTimer;
    }

    void switchStatus() {
        if (status == 1) {
            status = 0;

            runeCoords[0] = -1;
            runeCoords[1] = -1;
        }
        else {
            status = 1;
        }
    }

    void stopStatus() {
        if (status == 1) {
            status = 0;

            runeCoords[0] = -1;
            runeCoords[1] = -1;
        }
    }

    void startStatus() {
        if (status == 0) {
            status = 1;
        }
    }

    int status;
    int detectionStatus;
    int whiteIconUpdate;
    int mapBorderCoords[2];

    long randomPlayerSamePosTimer;
    long randomPlayerOldPosX;
    long randomPlayerOldPosY;

private:
    int currentPlayerCoords[2];
    int runeCoords[2];
    int whiteIconCoords[2];

    int randomPlayerInMap;
    long randomPlayerTimer;

    int friendPlayerInMap;
    long friendPlayerTimer;

    long whiteTimer;
    int runeclick;
    int portalclick;
    int runecapture;




};

