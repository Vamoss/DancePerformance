ofxRecorder
===========

![image](https://raw.github.com/Vamoss/ofxRecorder/master/ofxaddons_thumbnail.png)

Time based recorded events add-on for OpenFrameworks


###Description
It was originally developed to record and reproduce OSC messages automatically.
But you can also record anything, it is very generic.

The data are saved into .txt files using a structure like this:
<pre>
  [TIMESTAMP][SPACE][SAVED DATA]
  [TIMESTAMP][SPACE][SAVED DATA]
</pre>

Example:
<pre>
  0:0:2,111 HEAD
  0:0:3,477 SHOULDER LEFT
  0:0:6,229 ELBOW LEFT
</pre>

###Credits
This code was adapted from [ofxSubtitles](https://github.com/patriciogonzalezvivo/ofxSubtitles/) developed by [Patricio González](https://github.com/patriciogonzalezvivo/).
