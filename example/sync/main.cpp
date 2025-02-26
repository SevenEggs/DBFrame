#include "DatabaseEnvFwd.h"
#include "Log.h"

#include "DatabaseEnv.h"
#include "DatabaseLoader.h"
#include "Implementation/SakilaDatabase.h"
#include "MySQLThreading.h"
int main()
{
    MySQL::Library_Init();

    DatabaseLoader loader;
    loader.AddDatabase(SakilaDatabase, 
                "192.168.116.128;3306;jinyx;123456;sakila",
                8,2);

    if(!loader.Load())
    {
        TC_LOG_ERROR("", "SakilaDatabase connect error");
    }

    TC_LOG_INFO("", "SakilaDatabase connect success");

    {
        SakilaDatabase.DirectExecute("insert into actor(first_name, last_name) values('ting', 'xiaomi')");
        SakilaDatabase.DirectExecute("insert into actor(first_name, last_name) values('cheng', 'xiaomi')");
        auto result = SakilaDatabase.Query("select actor_id, first_name, last_name, last_update from actor where actor_id = 1");
        if(!result)
        {
            TC_LOG_ERROR("", "select empty");
            return 1;
        }

        TC_LOG_INFO("", "1 actor_id={}, first_name={} last_name={}, last_update={}",
            (*result)[0].GetUInt8(), 
            (*result)[1].GetString(),
            (*result)[2].GetString(),
            (*result)[3].GetString()
            );
    }

    {
        auto *stmt = SakilaDatabase.GetPreparedStatement(SAKILA_SEL_ACTOR_INFO);
        stmt->setUInt8(0, 1);
        auto result = SakilaDatabase.Query(stmt);
        // 格式化日期时间为字符串
        std::chrono::system_clock::time_point date = (*result)[3].GetDate();

        // 将 time_point 转换为 time_t
        std::time_t date_time_t = std::chrono::system_clock::to_time_t(date);

        // 将 time_t 转换为 tm
        std::tm* date_tm = std::localtime(&date_time_t);

        // 格式化日期时间为字符串
        char date_str[20]; // "YYYY-MM-DD HH:MM:SS" 共 19 个字符，加上终止符 '\0' 共 20
        std::strftime(date_str, sizeof(date_str), "%Y-%m-%d %H:%M:%S", date_tm);
        TC_LOG_INFO("", "2 actor_id={}, first_name={} last_name={}, last_update={}",
            (*result)[0].GetUInt8(), 
            (*result)[1].GetString(),
            (*result)[2].GetString(),
            date_str);
    }   

    {
        auto result = SakilaDatabase.Query("select actor_id, first_name, last_name, last_update from actor where actor_id >= 1");
        if(result)
        {
            do{
                Field* filed = result->Fetch();

                TC_LOG_INFO("", "3 actor_id={}, first_name={} last_name={}, last_update={}",
                filed[0].GetUInt8(), 
                filed[1].GetString(),
                filed[2].GetString(),
                filed[3].GetString());
            }while(result->NextRow());
        }
    }

    SakilaDatabase.Close();
    MySQL::Library_End();
    return 0;
}