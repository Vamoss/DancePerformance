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
    "label": "refrsh",
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
    "x":0, "y":0,
    "width":.75, "height":.25,
    "address" : "/red",
},
{
    "name":"G",
    "type":"Slider",
    "x":0, "y":0,
    "width":.75, "height":.25,
    "address" : "/green",
},
{
    "name":"B",
    "type":"Slider",
    "x":0, "y":0,
    "width":.75, "height":.25,
    "address" : "/blue",
},

]

];