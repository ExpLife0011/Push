VOID Adl_Initialize();

UINT8 Adl_GetActivity();
UINT16 Adl_GetEngineClock();
UINT16 Adl_GetMemoryClock();
UINT16 Adl_GetVoltage();
UINT16 Adl_GetEngineClockMax();
UINT16 Adl_GetMemoryClockMax();
UINT16 Adl_GetVoltageMax();
UINT8 Adl_GetTemperature();
UINT32 Adl_GetFanSpeed();

VOID Adl_SetEngineClock(UINT16 EngineClock);
VOID Adl_SetMemoryClock(UINT16 MemoryClock);
VOID Adl_SetVoltage(UINT16 Voltage);
VOID Adl_SetMaxClocks();

