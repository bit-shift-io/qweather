#include <QJsonObject>
#include <QJsonArray>
#include "forecastmodel.h"
#include "weather.h"

ForecastModel::ForecastModel(QObject *parent)
    : QAbstractListModel(parent),
      mWeather(nullptr) // initialize list
{
}

int ForecastModel::rowCount(const QModelIndex &parent) const
{
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    if (parent.isValid() || !mWeather || mWeather->mForecastData.isEmpty())
        return 0;

    return mWeather->mForecastData["forecast"].toArray().size() - todayCount();
}

int ForecastModel::todayCount() const
{
    // check if today is in the forecast
    const QJsonArray forecast = mWeather->mForecastData["forecast"].toArray();
    QDate date = QDate().fromString(forecast[0].toObject()["date"].toString(), "yyyy-MM-dd");
    if (date == QDate::currentDate())
        return 1;
    return 0;
}



QVariant ForecastModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || !mWeather || mWeather->mForecastData.isEmpty())
        return QVariant();

    const QJsonArray forecast = mWeather->mForecastData["forecast"].toArray();
    const QJsonObject item = forecast[index.row() + todayCount()].toObject();

    switch (role) {
    case IconRole:
        return QVariant(item["forecast_icon_code"].toString());
    case DayRole: {
        QDate date = QDate().fromString(item["date"].toString(), "yyyy-MM-dd");
        return QVariant(date.toString("dddd"));
    }
    case RainRangeRole:
        return QVariant(item["precipitation_range"].toString());
    case RainProbabilityRole:
        return QVariant(item["probability_of_precipitation"].toString());
    case TemperatureMinimumRole:
        return QVariant(item["air_temperature_minimum"].toString());
    case TemperatureMaximumRole:
        return QVariant(item["air_temperature_maximum"].toString());
    case DescriptionRole:
        return QVariant(item["precis"].toString());
    }
    return QVariant();
}


QHash<int, QByteArray> ForecastModel::roleNames() const
{
    // map/bind qml to cpp
    QHash<int, QByteArray> names;
    names[IconRole] = "icon";
    names[DayRole] = "day";
    names[RainRangeRole] = "rainRange";
    names[RainProbabilityRole] = "rainProbability";
    names[TemperatureMinimumRole] = "temperatureMinimum";
    names[TemperatureMaximumRole] = "temperatureMaximum";
    names[DescriptionRole] = "description";
    return names;
}

Weather *ForecastModel::weather() const
{
    return mWeather;
}

void ForecastModel::setWeather(Weather *weather)
{
    beginResetModel();

    if (mWeather) {
        mWeather->disconnect(this);
    }

    mWeather = weather;

    if (mWeather) {
        connect(mWeather, &Weather::resultForecastFinished, this, &ForecastModel::updateForecast);
    }

    endResetModel();
}

void ForecastModel::updateForecast(QJsonObject xResult)
{
    // force update
    beginResetModel();
    endResetModel();
}
