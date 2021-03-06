#pragma once

#include "sqldatabaseraii.hpp"
#include "dbprovider.hpp"
#include "model.hpp"
#include "settings.hpp"
#include "serializedevent.hpp"

class PersistenceManagerDBProvider
{
    DBProvider provider;

    SqlDatabaseRAII getDatabase() const;

    PersistenceManagerDBProvider(const PersistenceManagerDBProvider &) = delete;
    PersistenceManagerDBProvider &operator=(const PersistenceManagerDBProvider &) = delete;

public:
    PersistenceManagerDBProvider(const Settings &settings);

    void TestConnection();

    ConfigurationSet GetConfigurationSet();

    ConfigurationParameterValueList GetConfigurationParameters(const QString &prefix);
    JobConfigurationParameterValueList GetJobConfigurationParameters(int jobId,
                                                                     const QString &prefix);

    KeyedMessageList UpdateConfigurationParameters(const ConfigurationUpdateActionList &actions,
                                                   bool isAdmin);
    KeyedMessageList
    UpdateJobConfigurationParameters(int jobId, const JobConfigurationUpdateActionList &actions);

    ProductToArchiveList GetProductsToArchive();
    void MarkProductsArchived(const ArchivedProductList &products);

    int SubmitJob(const NewJob &job);
    int SubmitTask(const NewTask &task);
    void SubmitSteps(const NewStepList &steps);

    void MarkStepPendingStart(int taskId, const QString &name);
    void MarkStepStarted(int taskId, const QString &name);
    bool MarkStepFinished(int taskId, const QString &name, const ExecutionStatistics &statistics);
    void MarkStepFailed(int taskId, const QString &name, const ExecutionStatistics &statistics);

    void MarkJobPaused(int jobId);
    void MarkJobResumed(int jobId);
    void MarkJobCancelled(int jobId);
    void MarkJobFinished(int jobId);
    void MarkJobFailed(int jobId);
    void MarkJobNeedsInput(int jobId);

    TaskIdList GetJobTasksByStatus(int jobId, const ExecutionStatusList &statusList);
    JobStepToRunList GetTaskStepsForStart(int taskId);
    JobStepToRunList GetJobStepsForResume(int jobId);

    StepConsoleOutputList GetTaskConsoleOutputs(int taskId);

    void InsertEvent(const SerializedEvent &event);

    UnprocessedEventList GetNewEvents();
    void MarkEventProcessingStarted(int eventId);
    void MarkEventProcessingComplete(int eventId);

    void InsertNodeStatistics(const NodeStatistics &statistics);

    int InsertProduct(const NewProduct &product);
    ProductList GetProducts(int siteId, int productTypeId, const QDateTime &startDate, const QDateTime &endDate);
    ProductList GetProductsByInsertedTime(int siteId, int productTypeId, const QDateTime &startDate, const QDateTime &endDate);
    Product GetProduct(int siteId, const QString &productName);
    ProductList GetProductsForTile(int siteId, const QString &tileId, ProductType productType, int satelliteId, int targetSatelliteId);
    TileList GetSiteTiles(int siteId, int satelliteId);
    TileList GetIntersectingTiles(Satellite satellite, const QString &tileId);

    QString GetDashboardCurrentJobData(int page);
    QString GetDashboardServerResourceData();
    QString GetDashboardProcessorStatistics();
    QString GetDashboardProductAvailability(const QDateTime &since);
    QString GetDashboardJobTimeline(int jobId);

    QString GetDashboardProducts(const DashboardSearch &search);
    QString GetDashboardSites();
    QString GetDashboardSentinelTiles(int siteId);
    QString GetDashboardLandsatTiles(int siteId);
    QString GetDashboardProcessors();

    ProcessorDescriptionList GetProcessorDescriptions();
    QString GetProcessorShortName(int processorId);
    SiteList GetSiteDescriptions();
    QString GetSiteName(int siteId);
    QString GetSiteShortName(int siteId);

    std::vector<ScheduledTask> GetScheduledTasks( );
    void UpdateScheduledTasksStatus( std::vector<ScheduledTaskStatus>& taskList);

    void InsertScheduledTask( ScheduledTask& task);

    SeasonList GetSiteSeasons(int siteId);
};
