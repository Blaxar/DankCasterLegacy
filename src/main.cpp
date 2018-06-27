#include <iostream>

using namespace dankcaster;
using namespace dankcaster::sources;
using namespace std;

int main(int argc, char* argv[]){

	/* First sample scenario */

    Context ctx(1920,1080, 60, SND_STEREO,48000);

	Source webcam = ctx.createSource<V4l2>("/dev/video0");
	Source x11window = ctx.createSource<X11>("windowname");
	Source sndCapture = ctx.createSource<Alsa>("loopback");
	Source mic = ctx.createSource<Alsa>("U0x46d0x81b");

	Scene scene0 = ctx.createScene("optional name");
	Scene scene1 = ctx.createScene("optional name");

	WrpSource wrpWebcam0 = scene0.addSource(webcam);
	WrpSource wrpWindow0 =  scene0.addSource(x11Window);
	WrpSource wrpSnd0 = scene0.addSource(sndCapture);
  WrpSource wrpMic = scene0.addSource(mic);

	WrpSource wrpWebcam1 = scene1.addSource(webcam);
	WrpSource wrpWindow1 =  scene1.addSource(x11Window);
	WrpSource wrpSnd1 = scene1.addSource(sndCapture);
  WrpSource wrpMic1 = scene1.addSource(mic);

	/* Second sample scenario */

	Context ctx(1920,1080, 60, SND_STEREO,48000);

	Source webcam, x11window, sndCapture, mic;
	tuple sourcePack = ctx.createSources<V4l2, X11, Alsa, Alsa>("/dev/video0", "windowname", "loopback", "U0x46d0x81b");
	tie(webcam, x11window, sndCapture, mic) = sourcePack;
	
	return 0;
	
}
