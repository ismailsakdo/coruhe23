# CoRUHE 2023

## Chapter 1
### Hardware

## Chapter 2
### Arduino Sketch
1) Find the ADC value correspond to the voltage
2) Find the value with MAP Function [Link](https://raw.githubusercontent.com/ismailsakdo/coruhe23/main/SerialReadAnalog.ino)

### Arduino Sketch (Many Sensor)
1) Install Sensor
2) FInd the Sketch Example []().

## Chapter 3
### Cloud IOT (Google Sheet)
1) Google Sheet creation
2) Google App Script
3) Code for App Script [Link](https://raw.githubusercontent.com/ismailsakdo/coruhe23/main/ThingspeakGoogleSheet.gs)

## Chapter 4
### App Sheet (Summary Applications)
1) Create Table in Google Sheet
2) Open App Script
3) Code of App Script [Link](https://raw.githubusercontent.com/ismailsakdo/coruhe23/main/GoogleSheetAggregateExample.gs)
4) Import the Data and Make Summary for Hourly [Link](https://raw.githubusercontent.com/ismailsakdo/coruhe23/main/importDataSummaryHourly.gs)

## Chapter 5
### Environmental Data/ Secondary Data Analysis
1) Import Data "OpenWeather" [HERE](https://raw.githubusercontent.com/ismailsakdo/coruhe23/main/openweatherImport.gs)

## Chapter 6
### AppSheet Inspection and Investigation
#### List of function and codes:
Code to change the TEXT into NUMERICAL:
IF([HACCP1 - Food Storage] = "Poor", -1, IF([HACCP1 - Food Storage] = "Average", 1, IF([HACCP1 - Food Storage] = "Good", 2, "")))
IF([HACCP2 - Food Preparation] = "Poor", -2, IF([HACCP2 - Food Preparation] = "Average", 2, IF([HACCP2 - Food Preparation] = "Good", 4, "")))
IF([HACCP3 - Food Hygiene] = "Poor", -1, IF([HACCP3 - Food Hygiene] = "Average", 1, IF([HACCP3 - Food Hygiene] = "Good", 2, "")))

CONVERT NUMERIC INTO TEXT:
IFS(
  [TotalScore] < 0, "Closed",
  AND([TotalScore] >= 1, [TotalScore] <= 4), "Cleaning",
  [TotalScore] >= 5, "Operate",
  TRUE, "Unknown"
)

=====PDF===

concatenate("/appsheet/data/InspectionApps-744365623/inspectionppkp/",[id],".pdf")

========RENAME ID =====

SUBSTITUTE(CONCATENATE([Assessor], "-", [Date], " ", [Time], "-", [ID]), "/", "")

=========

SHARE LINK OF THE PUBLIC APPS: [LINK](https://www.appsheet.com/newshortcut/ccd0d390-f276-4798-a126-9ec9d98acef7)

## Chapter 7
#### MIT APP Inventor Image Classifier
1) Extension [LINK](https://github.com/ismailsakdo/coruhe23/blob/main/br.ufsc.gqs.teachablemachineimageclassifier.aix)
2) Example of the Apps [LINK](https://github.com/ismailsakdo/coruhe23/blob/main/Rubbish_App.aia)
