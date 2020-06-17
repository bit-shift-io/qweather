// see here for example
// https://github.com/mitchcurtis/todo-list-tutorial/tree/chapter-11
// https://www.youtube.com/watch?v=9BcAYDlpuT8

#ifndef FORECASTMODEL_H
#define FORECASTMODEL_H

#include <QAbstractListModel>

class Weather;

class ForecastModel : public QAbstractListModel
{
    Q_OBJECT
    // make aware of the weather class which provides the data
    Q_PROPERTY(Weather *weather READ weather WRITE setWeather)

public:
    explicit ForecastModel(QObject *parent = nullptr);

    // list of accesable properties/roles/map/bindings
    enum {
        IconRole,
        DayRole,
        RainRangeRole,
        RainProbabilityRole,
        TemperatureMinimumRole,
        TemperatureMaximumRole,
        DescriptionRole,
    };

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int todayCount() const;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    // hashmap to store properties/roles
    virtual QHash<int, QByteArray> roleNames() const override;

    Weather *weather() const;
    void setWeather(Weather *weather);
    void updateForecast(const QJsonObject &xResult);

private:
    Weather *mWeather;
};

#endif // FORECASTMODEL_H
