(*[[
    UserHelpURL= "ATCMfile:LCD_set_backlight.HTML",
]]*)
FUNCTION LCD_set_backlight : UINT
VAR_INPUT
	DutyCycle : UINT; (*[ LCD PWM DutyCycle ]*)
END_VAR
	INLINE {STDLIB(LCD_set_backlight)}
END_FUNCTION
