#include "utils/core/op/Token.hh"
#include "utils/core/auth/AuthorizationCodeGrant.hh"
#include "utils/core/auth/ClientCredential.hh"
#include <cJSON.h>
#include <log.hh>

namespace Utils
{
    namespace Op
    {

        std::string get_token(int token_type)
        {
            std::string result;
            if((Utils::Commons::get_file_size(file_target_Strings[token_type]) == -1) | Utils::Commons::check_expire(file_target_Strings[token_type]))
            {

                std::string response = (token_type) ? Utils::OAuth::get_client_credential_token() : Utils::OAuth::get_authorization_code_grant_token();

                if(response.find("code has expired") != std::string::npos)
                {
                    LOG("Code has expired, will need re-Authorize.");
                    std::remove(file_target_Strings[Utils::Commons::CODE]);
                    response = Utils::OAuth::get_authorization_code_grant_token();
                }

                cJSON* root = cJSON_Parse(response.c_str());
                cJSON* token = cJSON_GetObjectItem(root, "access_token");

                if(root == NULL)
                {
                    LOG("Unable to get token.");
                    result = "";
                }

                else
                {
                    result = token->valuestring;
                    write_token(token_type, result);
                }

                cJSON_Delete(root);
            }

            else
            {
                result = read_token(token_type);
            }

            return result;
        }

        std::string read_token(int token_type)
        {
            return Utils::Commons::read_file(file_target_Strings[token_type]);
        }

        void write_token(int token_type, const std::string& token_string)
        {
            Utils::Commons::write_file(file_target_Strings[token_type], token_string, "w+");
        }
    }// namespace Op
}// namespace Utils
