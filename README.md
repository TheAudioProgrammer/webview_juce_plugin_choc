This is the most basic "hello world" implementation of a JUCE plug-in with a JavaScript / HTML / CSS user interface. 

I use the Choc library to interface C++ / JUCE code with the webview.

In this code I'm using an AudioProcessorValueTreeState to register GAIN as a parameter.  I then route data bi-directionally - from the UI (JavaScript) to the Processor (C++) and vice versa.
