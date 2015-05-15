#include "persistencemanagerdbprovider.hpp"

SqlDatabaseRAII PersistenceManagerDBProvider::getDatabase() const
{
    return provider.getDatabase(QStringLiteral("PersistenceManager"));
}

ConfigurationSet PersistenceManagerDBProvider::GetConfigurationSet()
{
    auto db = getDatabase();

    return provider.handleTransactionRetry(QStringLiteral("GetConfigurationSet"), [&]() {
        auto query = db.prepareQuery(QStringLiteral("select * from sp_get_configuration_set()"));

        query.setForwardOnly(true);
        if (!query.exec()) {
            throw_query_error(query);
        }

        auto dataRecord = query.record();
        auto keyCol = dataRecord.indexOf(QStringLiteral("key"));
        auto categoryCol = dataRecord.indexOf(QStringLiteral("category"));
        auto friendlyNameCol = dataRecord.indexOf(QStringLiteral("friendly_name"));
        auto dataTypeCol = dataRecord.indexOf(QStringLiteral("data_type"));
        auto valueCol = dataRecord.indexOf(QStringLiteral("value"));
        auto isAdvancedCol = dataRecord.indexOf(QStringLiteral("is_advanced"));

        ConfigurationSet result;
        while (query.next()) {
            result.parameters.append({ query.value(keyCol).toString(),
                                       query.value(categoryCol).toInt(),
                                       query.value(friendlyNameCol).toString(),
                                       query.value(dataTypeCol).toString(),
                                       query.value(valueCol).toString(),
                                       query.value(isAdvancedCol).toBool() });
        }

        return result;
    });
}

ConfigurationParameterValueList
PersistenceManagerDBProvider::GetConfigurationParameters(const QString &prefix)
{
    auto db = getDatabase();

    return provider.handleTransactionRetry(QStringLiteral("GetConfigurationParameters"), [&]() {
        auto query = db.prepareQuery(
            QStringLiteral("select * from sp_get_configuration_parameters(:prefix)"));
        query.bindValue(":prefix", prefix);

        query.setForwardOnly(true);
        if (!query.exec()) {
            throw_query_error(query);
        }

        auto dataRecord = query.record();
        auto keyCol = dataRecord.indexOf(QStringLiteral("key"));
        auto valueCol = dataRecord.indexOf(QStringLiteral("value"));

        ConfigurationParameterValueList result;
        while (query.next()) {
            result.append({ query.value(keyCol).toString(), query.value(valueCol).toString() });
        }

        return result;
    });
}

KeyedMessageList PersistenceManagerDBProvider::UpdateConfigurationParameters(
    const ConfigurationParameterValueList &parameters)
{
    auto db = getDatabase();

    return provider.handleTransactionRetry(QStringLiteral("UpdateConfigurationParameters"), [&]() {
        auto query = db.prepareQuery(
            QStringLiteral("select * from sp_update_configuration_parameters(:parameters)"));
        query.bindValue(QStringLiteral(":parameters"), toJson(parameters));

        query.setForwardOnly(true);
        if (!query.exec()) {
            throw_query_error(query);
        }

        auto dataRecord = query.record();
        auto keyCol = dataRecord.indexOf(QStringLiteral("key"));
        auto errorCol = dataRecord.indexOf(QStringLiteral("error"));

        KeyedMessageList result;
        while (query.next()) {
            result.append({ query.value(keyCol).toString(), query.value(errorCol).toString() });
        }

        return result;
    });
}