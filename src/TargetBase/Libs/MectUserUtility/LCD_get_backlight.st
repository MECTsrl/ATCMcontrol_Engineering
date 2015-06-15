(*[[
    UserHelpURL= "ATCMfile:LCD_get_backlight.HTML",
]]*)
FUNCTION LCD_get_backlight : UINT
VAR_OUTPUT
	DutyCycle : UINT; (*[ LCD PWM DutyCycle ]*)
END_VAR
	INLINE {STDLIB(LCD_get_backlight)}
END_FUNCTION

