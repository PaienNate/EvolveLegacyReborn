# Information

The game's ca-bundle.crt should the same as the mitmproxy-ca-cert.pem in certs folder. Or 2K server crack will not work.



Game has three server. I named them:
- Verify server, 2k server, update server.
Verify server will be used when you click "Multiplayer"
- 2k server need fake ca-bundle.crt to work, and will unlock your monster and characters.
- update server will make your game to version 31 (If don't have, version will be 0, Like @Bluem describe below:
> hmm personally entering the hunting mode with a character like Tec Hack and using his shield, the difference is very visible, 
otherwise I would not know how to find out, the perk also affects
It looks like an old version of Evolve, the monsters do a grab and throw animation like the first versions used to do,
and I personally want to thank you for making this a reality. Thanks a lot



For use that, just run mitmserver.py. The fake server will run on 0.0.0.0:2000