/*
Copyright (c) 2018 Brian Lough. All right reserved.

HackadayIoApi - An Arduino library to wrap the Hackaday.Io API

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.
This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
Lesser General Public License for more details.
You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
*/

#ifndef HackadayIoApi_h
#define HackadayIoApi_h

#include <Arduino.h>
#include <ArduinoJson.h>
#include <Client.h>

#define HACKADAYIO_HOST "api.hackaday.io"
// Fingerprint correct as of Feb 3rd 2019
#define HACKADAYIO_FINGERPRINT "96 f1 71 61 71 4d e0 a3 a8 f8 60 56 53 5e 48 ff 62 47 7a 6c"
#define HACKADAYIO_TIMEOUT 2000

#define HACKADAYIO_PROJECTS_END_POINT "/v1/projects/%d?api_key=%s"

struct ProjectInfo
{
    int id;
    char *url;
    int owner_id;
    char *name;
    char *summary;
    //char *description ;
    char *image_url;
    long views;
    int comments;
    int followers;
    int skulls;
    //int logs;
    //int details;
    //int instruction;
    //int components;
    //int images;
    long created;
    long updated;
    //tags

    bool error;
};

class HackadayIoApi
{
  public:
    HackadayIoApi(Client &client, char *apiKey);
    bool makeGetRequest(char *command);
    ProjectInfo getProjectInfo(int projectId);
    int portNumber = 443;
    bool _debug = false;
    Client *client;

  private:
    char *_apiKey;
    void closeClient();
};

#endif