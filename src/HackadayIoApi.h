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

// root key for api.hackaday.io
// Common Name: DigiCert Global Root CA
// Organization: DigiCert Inc
// Organization Unit: www.digicert.com
// Country: US
// Valid From: November 9, 2006
// Valid To: November 9, 2031
// Issuer: DigiCert Global Root CA, DigiCert Inc
// Serial Number: 083be056904246b1a1756ac95991c74a
const char DIGICERTGLOBALROOTCA[] = R"=EOF=(
-----BEGIN CERTIFICATE-----
MIIDrzCCApegAwIBAgIQCDvgVpBCRrGhdWrJWZHHSjANBgkqhkiG9w0BAQUFADBh
MQswCQYDVQQGEwJVUzEVMBMGA1UEChMMRGlnaUNlcnQgSW5jMRkwFwYDVQQLExB3
d3cuZGlnaWNlcnQuY29tMSAwHgYDVQQDExdEaWdpQ2VydCBHbG9iYWwgUm9vdCBD
QTAeFw0wNjExMTAwMDAwMDBaFw0zMTExMTAwMDAwMDBaMGExCzAJBgNVBAYTAlVT
MRUwEwYDVQQKEwxEaWdpQ2VydCBJbmMxGTAXBgNVBAsTEHd3dy5kaWdpY2VydC5j
b20xIDAeBgNVBAMTF0RpZ2lDZXJ0IEdsb2JhbCBSb290IENBMIIBIjANBgkqhkiG
9w0BAQEFAAOCAQ8AMIIBCgKCAQEA4jvhEXLeqKTTo1eqUKKPC3eQyaKl7hLOllsB
CSDMAZOnTjC3U/dDxGkAV53ijSLdhwZAAIEJzs4bg7/fzTtxRuLWZscFs3YnFo97
nh6Vfe63SKMI2tavegw5BmV/Sl0fvBf4q77uKNd0f3p4mVmFaG5cIzJLv07A6Fpt
43C/dxC//AH2hdmoRBBYMql1GNXRor5H4idq9Joz+EkIYIvUX7Q6hL+hqkpMfT7P
T19sdl6gSzeRntwi5m3OFBqOasv+zbMUZBfHWymeMr/y7vrTC0LUq7dBMtoM1O/4
gdW7jVg/tRvoSSiicNoxBN33shbyTApOB6jtSj1etX+jkMOvJwIDAQABo2MwYTAO
BgNVHQ8BAf8EBAMCAYYwDwYDVR0TAQH/BAUwAwEB/zAdBgNVHQ4EFgQUA95QNVbR
TLtm8KPiGxvDl7I90VUwHwYDVR0jBBgwFoAUA95QNVbRTLtm8KPiGxvDl7I90VUw
DQYJKoZIhvcNAQEFBQADggEBAMucN6pIExIK+t1EnE9SsPTfrgT1eXkIoyQY/Esr
hMAtudXH/vTBH1jLuG2cenTnmCmrEbXjcKChzUyImZOMkXDiqw8cvpOp/2PV5Adg
06O/nVsJ8dWO41P0jmP6P6fbtGbfYmbW0W5BjfIttep3Sp+dWOIrWcBAI+0tKIJF
PnlUkiaY4IBIqDfv8NZ5YBberOgOzW6sRBc4L0na4UU+Krk2U886UAb3LujEV0ls
YSEY1QSteDwsOoBrp+uvFRTp2InBuThs4pFsiv9kuXclVzDAGySj4dzp30d8tbQk
CAUw7C29C79Fv1C5qfPrmAESrciIxpg0X40KPMbp1ZWVbd4=
-----END CERTIFICATE-----
)=EOF=";

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
    //char *description;
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
    time_t created;
    time_t updated;
    //char** tags;

    bool error;
};

class HackadayIoApi
{
  public:
    HackadayIoApi(Client &client, const char *apiKey);
    bool makeGetRequest(char *command);
    ProjectInfo getProjectInfo(int projectId);
    int portNumber = 443;
    int tagArraySize = 10;
    int addtionalStringSize = 1000;
    bool _debug = false;
    Client *client;

  private:
    const char *_apiKey;
    void closeClient();
};

#endif