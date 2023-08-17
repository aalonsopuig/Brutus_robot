<html>

<head>
<meta http-equiv="Content-Language" content="es">
<meta name="GENERATOR" content="Microsoft FrontPage 4.0">
<meta name="ProgId" content="FrontPage.Editor.Document">
</head>

<font size="4"><b><i>BRUTUS robot&nbsp;</i></b></font>
      <p>by Alejandro Alonso Puig<br>
      January 2.004<br>
<hr>
<p align="justify">I made Brutus with the idea of building a modular robot, as a platform to prove new experimental modules.<br />
<br />
The version 1.0 that is in this technical report refers to the simplest version of the implementation of the robot, in which it has wireless remote control of navigation, as well as a simple program for autonomous navigation.<br /></p>
<br />
<table align="center" cellpadding="0" cellspacing="0" class="tr-caption-container" style="margin-left: auto; margin-right: auto; text-align: center;"><tbody>
<tr><td style="text-align: center;"><a href="http://2.bp.blogspot.com/-GIQXROkLFQ8/Uu4hlpbgrFI/AAAAAAAAJfc/ilnMsLx8MZo/s1600/brutus.jpg" style="margin-left: auto; margin-right: auto;"><img border="0" height="300" src="http://2.bp.blogspot.com/-GIQXROkLFQ8/Uu4hlpbgrFI/AAAAAAAAJfc/ilnMsLx8MZo/s1600/brutus.jpg" width="400" /></a></td></tr>
<tr><td class="tr-caption" style="text-align: center;">Alejandro Alonso with his robot Brutus</td></tr>
</tbody></table>
<br />
Some basic characteristics that define Brutus v 1.0 are the following:<br />
<br />
<ul>
<li>Size (cm): 120 x 85 x 50</li>
<li>4 wheels&nbsp;</li>
<li>Two electrical DC motors with good torque attached to the rear wheels</li>
<li>Rear suspension</li>
<li>Front steering controlled by a good torque servo</li>
<li>Front sonar with controllable orientation&nbsp;</li>
<li>Shock Absorber (Fender/bumper) at the front with electronic shock detection</li>
<li>Onboard computer (Pentium) with control program in Visual C++</li>
<li>Module "Russian" for the management of the robot devices</li>
<li>Wireless communication WIFI with other external computer</li>
<li>3 batteries of 12v</li>
<li>System fully controlled via I2C bus</li>
</ul>
<br />
<div class="separator" style="clear: both; text-align: center;">
<p align="center"><img border="0" src="http://2.bp.blogspot.com/-p6THs-XQ0Zk/Uu4ijIGJIuI/AAAAAAAAJfo/LVEL2jRiVJw/s1600/brutus3.jpg" width="436" ></p>
</a></div></td>
<br />
In this work there will be mention to all the mechanical, electronic and programming aspects, as well as conclusions of the experience.<br />
<br />


<p align="justify">Information of interest:</p>
<ul>
  <li>
    <p align="justify"><a href="brutus_v1_doc.pdf">Technical report</a></li>
  <li>
    <p align="justify"><a href="brutuspict.zip">Multiple pictures of the robot</a></li>
  <li>
    <p align="justify"><a href="prog_Brutus_PC">Program for the onboard PC</a></li>
  <li>
    <p align="justify"><a href="https://github.com/aalonsopuig/Tarjeta_Interfaz_I2C_Puerto_paralelo.git">Repository of the KI2C board used in the project as interface between main onboard computer and other subsystems in the robot</a></li> 
  <li>
    <p align="justify"><a href="https://github.com/aalonsopuig/Servomotor_basico.git">Repository of the servomotor subsystem used to steer the robot</a></li>
 <li>
    <p align="justify"><a href="https://github.com/aalonsopuig/Sonar.git">Repository of the Sonar subsystem used in the front of the robot to detect obstacles</a></li>
<li>
    <p align="justify"><a href="https://www.youtube.com/v/H214qeLX6tY">Video of the robot going down stairs</a></li>
<li>
    <p align="justify"><a href="https://www.youtube.com/v/Z2_ZyfaXZWc">Another video of the robot doing crazy things</a></li>
</ul>

