loadedInterfaceName = "Color Control";

interfaceOrientation = "portrait";

pages = [[
{
    "name": "refresh",
    "type": "Button",
    "bounds": [.6, .9, .2, .1],
    "startingValue": 0,
    "isLocal": true,
    "mode": "contact",
    "ontouchstart": "interfaceManager.refreshInterface()",
    "stroke": "#aaa",
    "label": "refresh",
},
{
    "name": "tabButton",
    "type": "Button",
    "bounds": [.8, .9, .2, .1],
    "mode": "toggle",
    "stroke": "#aaa",
    "isLocal": true,
    "ontouchstart": "if(this.value == this.max) { control.showToolbar(); } else { control.hideToolbar(); }",
    "label": "menu",
},
{
    "name":"R",
    "type":"Slider",
    "x":0.05, "y":0.05,
    "width":0.9, "height":0.15,
    "address" : "RED",
    "min" : 0, "max" : 255,
},
{
    "name":"G",
    "type":"Slider",
    "x":0.05, "y":0.22,
    "width":0.9, "height":0.15,
    "address" : "GREEN",
    "min" : 0, "max" : 255,
},
{
    "name":"B",
    "type":"Slider",
    "x":0.05, "y":0.39,
    "width":0.9, "height":0.15,
    "address" : "BLUE",
    "min" : 0, "max" : 255,
},
{
    "name": "black",
    "label": "black",
    "type": "Button",
    "mode": "momentary",
    "x":0.05, "y":0.56,
    "width":0.44, "height":0.15,
    "stroke": "#aaa",
    "isLocal": true,
    "ontouchstart": "R.setValue(0); G.setValue(0); B.setValue(0);",
},
{
    "name": "white",
    "label": "white",
    "type": "Button",
    "mode": "momentary",
    "x":0.51, "y":0.56,
    "width":0.44, "height":0.15,
    "stroke": "#aaa",
    "isLocal": true,
    "ontouchstart": "R.setValue(255); G.setValue(255); B.setValue(255);",
},
{
    "name": "random",
    "label": "random",
    "type": "Button",
    "mode": "momentary",
    "x":0.05, "y":0.73,
    "width":0.44, "height":0.15,
    "stroke": "#aaa",
    "isLocal": true,
    "ontouchstart": "R.setValue(Math.random()*255); G.setValue(Math.random()*255); B.setValue(Math.random()*255);",
},

]

];