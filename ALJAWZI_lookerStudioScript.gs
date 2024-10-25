//Extract the Month ("Now" is your DateTime variable)
EXTRACT(MONTH FROM Now )

//Extract the Day
EXTRACT(DAY FROM Now )

//Extract the Hour
EXTRACT(Hour FROM Now )

//Extract the Minute
EXTRACT(MINUTE FROM Now )

//======= Hazard Score Example
//Make new Calculated Field for some issues/ hazards as 1 or 0
CASE WHEN [Your Column] > [Threshold Value] THEN 1 ELSE 0 END
