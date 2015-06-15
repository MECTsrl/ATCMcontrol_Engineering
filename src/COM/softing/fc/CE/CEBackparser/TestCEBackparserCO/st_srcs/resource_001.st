

RESOURCE Resource1 ON NT

    #IMPORT    Resource1.gvl    
    #IMPORT    abc.gvl    
    #IMPORT    def.gvl    

    TASK TaskCountTraffic (INTERVAL:=T#240ms, PRIORITY:=5);
    TASK TaskControlLights (INTERVAL:=T#500ms, PRIORITY:=6);
	TASK TaskCheckForOff (INTERVAL:=T#100ms, PRIORITY:=3);
	TASK TaskSimulate (INTERVAL:=T#100ms, PRIORITY:=4);
	TASK TaskCalculateSpeed	(INTERVAL:=T#1000ms, PRIORITY:=6);



    TASK Task1(INTERVAL:=T#100ms, PRIORITY:=5);
    TASK Task2(INTERVAL:=T#100ms, PRIORITY:=5);
    TASK Task3(INTERVAL:=T#100ms, PRIORITY:=5);
    TASK Task4(INTERVAL:=T#100ms, PRIORITY:=6);

    
	PROGRAM CountTraffic      WITH TaskCountTraffic: CountTraffic;    

    PROGRAM ControlLights     WITH TaskControlLights: ControlLights;
	PROGRAM retain C : CProg; 
	PROGRAM CheckForOff       WITH TaskCheckForOff: CheckForOff;
	PROGRAM Simulatea          WITH TaskSimulate: Simulate;
	PROGRAM Simulateb          WITH TaskSimulate: Simulate;
	PROGRAM Simulatec          WITH TaskSimulate: Simulate;
 	PROGRAM CalculateSpeed    WITH  TaskCalculateSpeed: CalculateSpeed;

END_RESOURCE

