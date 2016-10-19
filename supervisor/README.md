# Supervisor
The main module of the whole system. Controls every module, analysis the whole system in realtime.
### Capabilities
	* Controls the modules, their outputs and analyzes the output consistencies
	* Executes Leaving-Home last minute checks
	* Checks system health, sends heartbeat messages to every module in order to check if they are alive, or not
	* As an individual module, talks to the "Secretary" to store every action it took.