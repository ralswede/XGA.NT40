 
 Runstream (R) XGA/XGA-2 XGA208/NT Windows NT 4.0 Display Driver
 Copyright (C) UnalZ, 2008, Runstream & PS/2 MCABASE
 -----------------------------------------------------------------
 Beta Test XGA208/NT                            unalz@mail333.com


   This document complements the XGA208/NT installation instructions
   and presents authentic test reports.

   The driver installation procedure is described in INSTALL.TXT.
  
   Driver updates and information at http://www.members.aon.at/mcabase

 
   CONTENTS
  
   1. Before You Start
         Multiple XGA/XGA-2 Adapters
         LED Display TV
   2. Before You Install
         XGA Aperture
         Create a Test Profile on Windows NT
   3. Resolutions and Monitor Specs      
   4. Test Reports
 
 
 1. BEFORE YOU START

   You need a MicroChannel machine with an XGA or XGA-2 display adapter
   and the Windows NT operating system. 
   
   The XGA208 miniport driver XGA2.SYS works with the Windows NT display 
   driver XGA.DLL. You must have the Windows NT XGA driver installed
   before you install the XGA208 miniport driver.

   To differentiate between the Windows NT XGA driver and the XGA208
   miniport driver, the installable types have been defined as:

   "IBM XGA MCA"    = IBM XGA Display Adapter with the XGA208 miniport driver.
   "IBM XGA-2 MCA"  = IBM XGA-2 Display Adapter with the XGA208 miniport driver.

   The Windows NT XGA driver defines the XGA and XGA-2 display types
   as "IBM XGA" and "IBM XGA-2".
   
   MULTIPLE XGA/XGA-2 ADAPTERS
   
   If you have multiple XGA or XGA-2 adapters, only one adapter will be 
   supported. The adapter selection rules are as follows:

    * Planar XGA-2 and one XGA/XGA-2 display adapter in an extension slot.
       
         The adapter in the extension slot is used.
       
    * Planar XGA-2 and multiple XGA/XGA-2 display adapters in extension slots.
    
         The adapter in the LOWEST numbered extension slot used.
         
    * Multiple XGA/XGA-2 display adapters in extension slots.
    
         The adapter in the LOWEST numbered extension slot will be used.

   The additional XGA-2 adapters are not disabled.

   
   LED DISPLAY TV

   The driver package includes a "television" version of the XGA208 miniport
   driver. It can be used on PS/2 Mod. 95 with a LED display panel. The driver
   will output status messages to the panel. To use this version, make first a
   backup copy of the XGA2.SYS and then copy XGA2.LED to XGA2.SYS. 
   
 
 3. BEFORE YOU INSTALL
   
   XGA APERTURE
   
   Either the 1MB or 4MB XGA video memory aperture must be enabled. The 4MB 
   aperture is enabled per default on PS/2 systems supporting more than 16MB
   of system memory when the XGA resp. XGA-2 is inserted in a 32-bit slot.
   
   If necessary, enable the 1MB aperture using the reference disk. To check
   the status of the aperture, use the XGA-2 utilities, download from here:
  
    	www.members.aon.at/mcabase/pub/files/xga2util.zip
      
   When both apertures are disabled, the driver will exit and NT will start
   in VGA mode. You can deactivate XGA2 and activate XGA in your test profile.
      
     
   CREATE A TEST PROFILE ON WINDOWS NT
   
   Follow the outlined procedure to avoid driver conflicts and problems:
    
    - You must have the Windows NT XGA display driver installed before you begin.
    - Make an emergency repair disk with RDISK (enter RDISK)
    - Go to Settings / Control Panel / System. 
    - Create a hardware test profile, e.g. "XGA208 Test Configuration"
    - Boot NT with the created test profile
    - Go to Settings / Control Panel / Devices
    - Search for XGA, find it and click on HW-Profile
    - DEACTIVATE it in the "XGA208 Test Configuration"
    
    - Unzip the downloaded package. 
    - The miniport driver files must be in the directory XGA208NT
    - Install the miniport, same procedure as on W9x, see below
    - Do NOT restart your computer when you are prompted by NT. 
    
    - Go to Settings / Control Panel / Devices
    - Search for XGA2, find it and click on HW-Profile
    - ACTIVATE it in the profile "XGA208 Test Configuration"
    - DEACTIVATE in all other profiles
    
    - Restart your computer
    - Select "XGA208 Test Configuration" profile to boot NT
    - The display will come in VGA mode
    - Select the desired mode
   
   IMPORTANT

   Remember to check the activation status of the XGA and XGA2 drivers
   in the system applet "Devices".
   
   
 3. RESOLUTIONS AND MONITOR SPECS
   
   The XGA-2 Display Adapter is a highly programmable device. The extended
   graphics (XGA) video modes are programmed and not contained in the BIOS.
    
   The XGA208/NT miniport uses monitor timings, or XGA display mode settings 
   which require assumed monitor specifications. EIZO FlexScan T56S was used
   as a reference monitor. All XGA208 display modes do not require any
   adjustment on the EIZO FlexScan T56S. Your monitor, however, may eventually
   need some adjustment. XGA208/NT does not implement the Display Mode Query
   and Set (DMQS) mode of operation.
      
   The XGA208/NT miniport driver supports 4/8/16-bpp display modes on the
   XGA/XGA-2 resulting in modes with 16/256/65536 colors. The Windows NT XGA
   display driver, for which the XGA208 miniport provides the XGA hardware
   interface services, selects only the 8-bpp 256 colors modes and ignores
   all other XGA208 modes. 
   
   The following tables contain only the 8-bpp 256 colors XGA208 modes usable
   on Windows NT with the MS Windows NT XGA display driver and the XGA208/NT
   XGA/XGA-2 miniport driver. Find the desired display resolution, color depth
   and compare the given line rate with the specification of your monitor. 
   All display modes are non-interlaced unless noted as interlaced.
   
   XGA-2 Modes -----------------------------------------------------------
   Resolution  Number   Frame  Listed    Line    PEL     Video   Monitor
   Pixels      Colors   Hz     Hz**      kHz     MHz     Mode    Notes 
   -----------------------------------------------------------------------
   640x480      256     75      75       39.4    --      101     Multisync
   640x480      256     72      72       37.9    --      101     Multisync
   640x480      256     60      60       31.6    --      101     Multisync 
   
   800x600      256     98.5   100       61.4    65      103     Multisync  
   800x600      256     94.8    95       59.0    62      103     Multisync  
   800x600      256     85.1    85       52.8    55.5    103     Multisync  
   800x600      256     73.6    75       45.7    48*     103     Multisync  
   
   832x620      256     99.4   100       64.2    70      162     Multisync
   832x620      256     95.2    95       61.5    67      162     Multisync
   832x620      256     85.2    85       55.0    60      162     Multisync
   832x620      256     75.3    75       48.6    53      162     Multisync
   
   960x720      256     84.6    85       63.2    81      164     Multisync
   960x720      256     81.4    82       60.8    78      164     Multisync
   960x720      256     79.6    80       60.9    84      164     IBM 9515
   
   1024x768     256     84.6    85       67.2    90      105     Multisync
   1024x768     256     75.0    75       61.1    86      105     IBM 9515 
   1024x768     256     70.0    70       56.5    75      105     Multisync
   1024x768     256     60.0    60       48.4    65      105     Multisync
   
   1040x768     256     83.1    85       66.0    90      165     Multisync
   1040x768     256     75.2    75       60.6    87      165     IBM 9515 
   
   1104x828     256     78.1    78       66.9    96*     166     Multisync 
   1104x828     256     75.6    75       64.8    93*     166     Multisync 
   1104x828     256     73.2    72       62.7    90      166     Multisync 

   1120x840     256     74.5    75       64.8    94*     167     Multisync
   1120x840		256     71.3    70       62.0    90      167     Multisync
   
   1160x870     256     69.7    70       62.7    94*     168     Multisync
   1160x870     256     66.7    65       60.0    90      168     Multisync

   1280x800     256     75.2    75       62.1    102*    169     WIDE Multisync
   -----------------------------------------------------------------------
    * = Uses high PEL rate. Some XGA-2 adapters may not be able to function
        without errors. Do not use the mode if you wish to strictly comply
        with the IBM specifications published in 1992.
   ** = The refresh rate value which is listed in the Display Applet.


   XGA Modes -------------------------------------------------------------
   Resolution  Number   Frame  Listed    Line    PEL     Video   Monitor
   Pixels      Colors   Hz     Hz**      kHz     MHz     Mode    Notes 
   -----------------------------------------------------------------------
   640x480      256     65      65       31.8    25.25   101     Multisync
      
   800x600*     256     70.0    70       43.2    45      103     Multisync  
   
   832x620*     256     64.4    65       41.3    45      162     Multisync
   
   960x720*     256     50.0 I  50       37.4    45      164     Multisync
   
   1024x768     256     44.7 I  45       35.5    45      105     Multisync
   -----------------------------------------------------------------------
    I = Interlaced
    * = Introduced for the first time with the XGA208 display driver. These
        modes are not listed in the IBM technical specifications.
   ** = The refresh rate value which is listed in the Display Applet.
        
   
 4. TEST REPORTS
   
   This section contains selected excerpts from the message exchanges in the
   COMP.SYS.IBM.PS2.HARDWARE newsgroup during the development and testing phase
   of the XGA208/NT video miniport driver.
     

   UNALZ (April 1, 2008) 
   
   The display driver (NT) and the miniport (XGA208) communicate through a
   private structure. If I cannot guess it right, the melange trick will not
   work. Actually, I should move the miniport first to DOS (it is surprisingly
   simple to port an NT miniport to DOS), and test what can be tested there,
   like detecting the card and setting the modes.
   
   
   UNALZ (April 3, 2008)
   
   I did guess it right, so I think. There is a light, display light coming
   from the XGA208/NT miniport, this is the good news. The bad news is that
   the whole picture is squeezed in 1/12th of the screen height, that is,
   20 mm out of 240 mm are visible (this is 64 pixels high, i.e. 1024x64
   at 1024x768).
   
   The visible stripe is on the top of the screen. I even managed to run a DOS
   command prompt window there, with only 4 visible lines of output. I need to
   find a command line utility dumping the display mode parameters to a file.
   
   I use the LED panel display on Mod. 95 again as a monitor, it dumps the
   miniport debug data. If you want to test a 64 pixels high display, tell me.
   
   > > The display has now the full height.
   > What was the problem? (William R. Walsh)
   
   The problem was with bank switching. The bank switching code is a real hack,
   employed both by W9x and NT. It is a piece of assembly language code at the
   miniport side that is modified and copied to the display driver side for
   performance reasons. Bank switching is actually not needed when the 1M/4M
   aperture is enabled, so I disabled it and hard coded the register address
   for Instance 6 to test when needed (it is needed only when the apertures are
   disabled). This fixed the problem, and I am going to fix it for any XGA
   instance.
   
   For the display driver interface, "NT vs. W9x" is in fact "NT equals W9x".
   Conceptually and technically, there are almost no differences, different are
   the structures and a few details. NT is simpler, W9x requests the VESA
   interface and is more complicated. If you know how to do a display driver on
   W9x, you will have no problems doing it on NT.
 

   UNALZ (April 4, 2008)

   XGA208/NT Miniport driver for Windows NT, Release A
   ----------------------------------------------------
   
   I am really satisfied and happy with the result. The XGA208 Windows NT
   miniport is finally there. The display driver will follow one day.

   The XGA208 miniport driver enables 800x600 and 832x620 with 256 colors on
   the XGA display adapter on Windows NT. The 800x600 and 832x620 64K colors
   modes and the 16 colors high resolution modes, all natively supported by the
   XGA208, are unfortunately filtered out by the NT XGA display driver.
   

   JIM SHORNEY (April 6, 2008)
   
   System:
   Server 95a (Array), 9595-3QT
   90 MHz Pentium (unmodified)
   80 MB ECC RAM
   4 GB in one physical RAID5 array (6x 0662 1G narrow drives)
   NT Server 4.0 SP6
   Sony 100GS Trinitron monitor
   White RAMDAC XGA2, no heatsink (it was the first one I found in the box...)
   
   Following UZ's instructions to the letter, installation of XGA208nt 
   release A went without a hitch (about time something did, thanks UZ!). 
   Immediately after reboot, I was presented with a quite usable and 
   well-centered display.
   
   Performance seems good. A quick run-through of all modes listed revealed 
   no major problems, although some modes were off-centered rightwards. 
   I'll do more thorough testing later, it's time to knock off for the night.
   
   Nicely done, UZ!


   UNALZ (April 12, 2008)

   XGA208/NT Miniport Release B.
   Fly Gagarin, fly.
   
   The 4M video memory aperture must be enabled. It is auto enabled in a 32-bit
   slot in a 32-bit system. The case where only the 1M aperture is enabled,
   although implemented, is not yet tested. When both apertures are disabled,
   the driver will not load. The adapter can be set now to any instance.
   
   Release B
   2008/04/12: www.members.aon.at/mcabase/pub/files/xga208ntb.zip
   
   I would like to thank Jim Shorney for testing the driver and confirming the
   results. I thank the audience for the valuable attention and discussions.
   

   JIM SHORNEY (April 13, 2008)

   It flies. Installation was without problems, as usual. The LED panel 
   mode seems to take a hit on performance - there are long pauses of black 
   screen between mode changes. Maybe because I have the ps2panel driver 
   installed also? Reverting to the non-panel version of XGA2.SYS 
   eliminates the pauses. 
   
   > The LED panel version inserts a delay after each message
      
   The image is stable and well centered. This machine (Server 95a Array 
   model, P-90 processor) had been running continuously since installation 
   of the previous version of XGA208/NT, and no problems had been noted. 
   The Display Type panel indicates XGA-2, Instance 6 Slot 5, 1 MB RAM, 4 
   MB aperture enabled.
   
   However, I only see 8-BPP color modes in the list of supported modes and 
   in the selection drop-down. Documentation indicates that 16-BPP is 
   available. UZ, where are my missing bits?
   
   > The miniport (XGA2.SYS) supplies the modes but the MS display driver
   > (XGA.DLL) selects only the 8-bpp modes and rejects all others. Since we
   > are only the miniport driver - the part of the display driver package
   > which interfaces to the hardware, inits the adapter and sets the display
   > modes - the final choice and handling of the graphics operations is the
   > decision of the MS display driver.
   
   
   WILLIAM R. WALSH (April 13/14, 2008)
   
   9585-0X0 S/N 23AA009
   XGA-2 with white RAMDAC, no heatsink
   Quantum Maverick 540S hard disk
   32MB RAM
   Soldered 486SX-33 CPU
   Goldstar 1460DL 14" SVGA monitor
   
   After following the directions to install the driver and make the hardware 
   profiles, Windows NT was restarted with the new profile and driver in 
   effect. Windows NT booted up, and the monitor's power LED turned orange 
   shortly after the "blue startup screen" disappeared, which it had not done 
   before. I was afraid that the card had gone into a mode that the monitor 
   simply could not display.
   
   > Nothing to worry about, NT issues the reset request and the card
   > switches to VGA mode. The new mode is not immediately set and the card
   > cuts off the video signal. This behaviour is expected.
   
   This did not end up being the case. The monitor came on after a short delay 
   and the Windows NT login screen appeared. It should be noted that the system 
   came up in 640x480 but with 256 colors instead of 16. The Windows NT display
   panel appeared with a dialog stating that a new graphics driver had been 
   installed.
   
   > The default NT VGA driver uses 16 colors. BTW, once you have set up your
   > profiles and set the device activation/deactivation status, you do not
   > need to repeat it on every new install.  
   
   I tried to get at some of the higher resolution modes, but the only scan 
   rate I am allowed to select is the 75Hz one. This will not work with my 
   monitor, so I will have to select another one for further testing. I did try 
   to enter these modes, but the video was broken up and rolling on the screen 
   since it cannot sync at high refresh rates.
   
   > When the XGA2 miniport is loaded, you should be actually able to select
   > all modes. I have not experienced this case.
   
   I can select all the resolutions, and they work great. It's selecting scan
   rates that is a bit more restrictive. The XGA-2 do everything that is
   listed when the "List All Modes" button is clicked. Selecting anything
   (resolution+scan rate combo) outside of that list will force the resolution
   slider back to 640x480 at 60Hz.
   
   > What do you see when you "List all modes" ?
   
   640x480,  256 colors 60/72/75
   640x510,  256 colors 75
   800x600,  256 colors 75/85/95/100 (are the last two interlaced?)
   832x620,  256 colors 75/85/95/100
   960x720,  256 colors 80/82/85/95
   1024x768, 256 colors 60/70/75/85
   1040x768, 256 colors 75/85
   1104x828, 256 colors 72/75/78
   1120x840, 256 colors 70/75
   1160x870, 256 colors 60/70
   1280x800, 256 colors 75
   
   > All 800x600 modes are non-interlaced.
   

   WILLIAM R. WALSH (April 14/15, 2008)

   I dug out the Monster Monitor--a NEC MultiSync XE21. This thing is a huge 
   unit. NT came up, the Token Ring adapter inserted into the network and
   I logged on. Once my desktop had loaded, the NT Display Control Panel
   came up and told me that the currently selected display resolution was
   invalid. Hmmm...does the new XGA-208NT probe the monitor or use DMQS?
   
   > No, it does not. But the card itself always probes the monitor and
   > computes the monitor ID. This ID cannot be retrieved on NT through the
   > XGA BIOS DMQS call because of the NT video miniport limitations. 
   > The miniport cannot pass a memory buffer to a BIOS call but the
   > DMQS call needs such a buffer to copy the info into it.

   ...and now the results (several minutes later)...
   
   All modes result in a usable, stable picture. The monitor's controls have 
   more than enough range to get a nice centered desktop on every resolution 
   and every scan rate. I've always thought highly of NEC monitors, and while 
   this one is showing its age, it does a nice job.
   
   I am curious - is there any support for using an attached 8514/8515
   (1024x768  43Hz interlaced) display with XGA-208? I may have to move
   the 9517 in here for some more testing...
   
   > There is an interlaced 1024x768 support only for the XGA,
   > not for the XGA-2.
   
   
   JIM SHORNEY (April 6/15, 2008)
   
   XGA-2 is not supported in the 720 (Ed: IBM PC Server 720, Mod. 8642)
   
   SGVA subsytem is [Disabled], all the XGA-2  settings are visible and
   tweakable. But the damn Cirrus won't let go.
   
   > The Cirrus is basicly a VGA card, I have just checked the Cirrus miniport
   > code, and it uses and claims the VGA address ranges and a few extra ports
   > not used by the XGA. Usually, the XGA takes over the planar VGA (Cirrus in
   > this case) when you connect the monitor cable to the XGA adapter.
   
   Not it this case. I connected a SVGA monitor directly to the XGA2 port 
   and it remains dark. The system powers up and boots on the disabled Cirrus.
   
   I see that the 4 MB aperture is disabled. This system has half a gig of RAM,
   it should automagically turn on, right? Is that TOO MUCH for XGA2 to cope
   with, or is the Cirrus hack obfuscating the true nature of XGA2? 
   Of course, 1 MB aperture cannot be enabled without conflicts in SC.
   
   > The inability to enable the 4MB aperture could indicate that the XGA 4MB
   > aperture region is either not available (one 4M region for each XGA
   > instance, 32MB total) or cannot be set.
   
   I've always wondered about this particular 720. Others have reported no 
   problems running PCI video cards in the system, but I have tried several 
   different PCI video cards and cannot get NT to acknowledge anything but 
   a generic VGA subsystem. I.E., no SVGA or HiColor modes. It has been 
   most frustrating. 
   
   Planar Devices:
   ---------------
   Planar  1: FEE7  Planar ID - PS/2 Model 8642 Server 720
   Planar  2: ----  POS
   Planar  3: 917C  Video
   
   > The Cirrus has the adapter ID of 917C, previously unknown? 917B is
   > known as the SVGA adapter.
   
   > NT was able to detect the XGA. I think the problem is with the
   > disable-resistant Cirrus. Apparently the XGA cannot disable the planar VGA
   > in the usual way. Perhaps the fact that the Cirrus identifies itself as
   > 917C irritates the XGA.
   
   I'm glad this excercise is providing some useful information, at least. :)
   
   > Yes, it does. There happens something strange, video memory addressing
   > which worked on W95 does not on NT in the case of Mod. 57 with disabled
   > apertures. I could not get the XGA208/NT to work in Mod. 57. The XGA-2
   > memory is at the 56 MB address. W95 has no problems accessing.
   

END.
   

