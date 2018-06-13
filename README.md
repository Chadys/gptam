# GPTAM compiled to webassembly
 

This real-time Visual SLAM application is a rework of George Terzakis' GPTAM which is itself a deep code modification of the brilliant original work by Klein and Murray , "Parallel Tracking and Mapping" (PTAM)."
This is an attempt to have GPTAM compiled to Webassembly so it can be executed inside a web environment.
This project is born from the frustration of native application not being "plug & play" enough for me.
The only thing you need to have installed on your computer, beside a recent browser, are docker and docker-compose.
After that, just cd into the 'docker' directory and execute the command `docker-compose up`.
If you don't want to use docker, you can execute directly on your system all the actions done by the 'Dockerfile' and 'docker-compose.yml' but I don't recommend it.
At the address 'http://0.0.0.0:8080/' you'll find the .html files produced by the application.

**This is a work in progress, a lot of adjustments are needed to make it work, I'll detail all the steps I've taken and how to use the app once I'm done.**