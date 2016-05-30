<!DOCTYPE html>
<?php
   define('BASE_DIR', dirname(__FILE__));
   require_once(BASE_DIR.'/config.php');
   $config = array();
   $debugString = "";
   
   $options_mm = array('Average' => 'average', 'Spot' => 'spot', 'Backlit' => 'backlit', 'Matrix' => 'matrix');
   $options_em = array('Off' => 'off', 'Auto' => 'auto', 'Night' => 'night', 'Nightpreview' => 'nightpreview', 'Backlight' => 'backlight', 'Spotlight' => 'spotlight', 'Sports' => 'sports', 'Snow' => 'snow', 'Beach' => 'beach', 'Verylong' => 'verylong', 'Fixedfps' => 'fixedfps');
   $options_wb = array('Off' => 'off', 'Auto' => 'auto', 'Sun' => 'sun', 'Cloudy' => 'cloudy', 'Shade' => 'shade', 'Tungsten' => 'tungsten', 'Fluorescent' => 'fluorescent', 'Incandescent' => 'incandescent', 'Flash' => 'flash', 'Horizon' => 'horizon');
//   $options_ie = array('None' => 'none', 'Negative' => 'negative', 'Solarise' => 'solarise', 'Sketch' => 'sketch', 'Denoise' => 'denoise', 'Emboss' => 'emboss', 'Oilpaint' => 'oilpaint', 'Hatch' => 'hatch', 'Gpen' => 'gpen', 'Pastel' => 'pastel', 'Watercolour' => 'watercolour', 'Film' => 'film', 'Blur' => 'blur', 'Saturation' => 'saturation', 'Colourswap' => 'colourswap', 'Washedout' => 'washedout', 'Posterise' => 'posterise', 'Colourpoint' => 'colourpoint', 'ColourBalance' => 'colourbalance', 'Cartoon' => 'cartoon');
// Remove Colourpoint and colourbalance as they kill the camera
   $options_ie = array('None' => 'none', 'Negative' => 'negative', 'Solarise' => 'solarise', 'Sketch' => 'sketch', 'Denoise' => 'denoise', 'Emboss' => 'emboss', 'Oilpaint' => 'oilpaint', 'Hatch' => 'hatch', 'Gpen' => 'gpen', 'Pastel' => 'pastel', 'Watercolour' => 'watercolour', 'Film' => 'film', 'Blur' => 'blur', 'Saturation' => 'saturation', 'Colourswap' => 'colourswap', 'Washedout' => 'washedout', 'Posterise' => 'posterise', 'Cartoon' => 'cartoon');
   $options_ce_en = array('Disabled' => '0', 'Enabled' => '1');
   $options_ro = array('No rotate' => '0', 'Rotate_90' => '90', 'Rotate_180' => '180', 'Rotate_270' => '270');
   $options_fl = array('None' => '0', 'Horizontal' => '1', 'Vertical' => '2', 'Both' => '3');
   $options_bo = array('Off' => '0', 'Background' => '2');
   $options_av = array('V2' => '2', 'V3' => '3');
   $options_at_en = array('Disabled' => '0', 'Enabled' => '1');
   $options_ac_en = array('Disabled' => '0', 'Enabled' => '1');
   $options_ab = array('Off' => '0', 'On' => '1');
   $options_vs = array('Off' => '0', 'On' => '1');
   $options_rl = array('Off' => '0', 'On' => '1');
   $options_vp = array('Off' => '0', 'On' => '1');
   $options_mx = array('Internal' => '0', 'External' => '1');
   $options_mf = array('Off' => '0', 'On' => '1');
   $options_cn = array('First' => '1', 'Second' => '2');
   $options_st = array('Off' => '0', 'On' => '1');
   
   function initCamPos() {
      $tr = fopen("pipan_bak.txt", "r");
      if($tr){
         while(($line = fgets($tr)) != false) {
           $vals = explode(" ", $line);
           echo '<script type="text/javascript">init_pt(',$vals[0],',',$vals[1],');</script>';
         }
         fclose($tr);
      }
   }

   function pan_controls() {
      $mode = 0;
      if (file_exists("pipan_on")){
         initCamPos();
         $mode = 1;
      } else if (file_exists("servo_on")){
         $mode = 2;
      }
      if ($mode <> 0) {
         echo '<script type="text/javascript">set_panmode(',$mode,');</script>';
         echo "<div class='container-fluid text-center liveimage'>";
         echo "<div alt='Up' id='arrowUp' style='margin-bottom: 2px;width: 0;height: 0;border-left: 20px solid transparent;border-right: 20px solid transparent;border-bottom: 40px solid #428bca;font-size: 0;line-height: 0;vertical-align: middle;margin-left: auto; margin-right: auto;' onclick='servo_up();'></div>";
         echo "<div>";
         echo "<div alt='Left' id='arrowLeft' style='margin-right: 22px;display: inline-block;height: 0;border-top: 20px solid transparent;border-bottom: 20px solid transparent;border-right: 40px solid #428bca;font-size: 0;line-height: 0;vertical-align: middle;' onclick='servo_left();'></div>";
         echo "<div alt='Right' id='arrowRight' style='margin-left: 22px;display: inline-block;height: 0;border-top: 20px solid transparent;border-bottom: 20px solid transparent;border-left: 40px solid #428bca;font-size: 0;line-height: 0;vertical-align: middle;' onclick='servo_right();'></div>";
         echo "</div>";
         echo "<div alt='Down' id='arrowDown' style='margin-top: 2px;width: 0;height: 0;border-left: 20px solid transparent;border-right: 20px solid transparent;border-top: 40px solid #428bca;font-size: 0;line-height: 0;vertical-align: middle;margin-left: auto; margin-right: auto;' onclick='servo_down();'></div>";
         echo "</div>";
      }
   }
  
   function pilight_controls() {
      echo "<tr>";
        echo "<td>Pi-Light:</td>";
        echo "<td>";
          echo "R: <input type='text' size=4 id='pilight_r' value='255'>";
          echo "G: <input type='text' size=4 id='pilight_g' value='255'>";
          echo "B: <input type='text' size=4 id='pilight_b' value='255'><br>";
          echo "<input type='button' value='ON/OFF' onclick='led_switch();'>";
        echo "</td>";
      echo "</tr>";
   }

   function getExtraStyles() {
      $files = scandir('css');
      foreach($files as $file) {
         if(substr($file,0,3) == 'es_') {
            echo "<option value='$file'>" . substr($file,3, -4) . '</option>';
         }
      }
   }
   
  
   function makeOptions($options, $selKey) {
      global $config;
      switch ($selKey) {
         case 'flip': 
            $cvalue = (($config['vflip'] == 'true') || ($config['vflip'] == 1) ? 2:0);
            $cvalue += (($config['hflip'] == 'true') || ($config['hflip'] == 1) ? 1:0);
            break;
         case 'MP4Box': 
            $cvalue = $config[$selKey];
            if ($cvalue == 'background') $cvalue = 2;
            break;
         default: $cvalue = $config[$selKey]; break;
      }
      if ($cvalue == 'false') $cvalue = 0;
      else if ($cvalue == 'true') $cvalue = 1;
      foreach($options as $name => $value) {
         if ($cvalue != $value) {
            $selected = '';
         } else {
            $selected = ' selected';
         }
         echo "<option value='$value'$selected>$name</option>";
      }
   }

   function makeInput($id, $size, $selKey='') {
      global $config, $debugString;
      if ($selKey == '') $selKey = $id;
      switch ($selKey) {
         case 'tl_interval': 
            if (array_key_exists($selKey, $config)) {
               $value = $config[$selKey] / 10;
            } else {
               $value = 3;
            }
            break;
         case 'watchdog_interval':
            if (array_key_exists($selKey, $config)) {
               $value = $config[$selKey] / 10;
            } else {
               $value = 0;
            }
            break;
         default: $value = $config[$selKey]; break;
      }
      echo "<input type='text' size=$size id='$id' value='$value'>";
   }
   
   function getImgWidth() {
      global $config;
      if($config['vector_preview'])
         return 'style="width:' . $config['width'] . 'px;"';
      else
         return '';
   }
   
   if (isset($_POST['extrastyle'])) {
      if (file_exists('css/' . $_POST['extrastyle'])) {
         $fp = fopen(BASE_DIR . '/css/extrastyle.txt', "w");
         fwrite($fp, $_POST['extrastyle']);
         fclose($fp);
      }
   }
   
   $toggleButton = "Simple";
   $displayStyle = 'style="display:block;"';
   if(isset($_COOKIE["display_mode"])) {
      if($_COOKIE["display_mode"] == "Simple") {
         $toggleButton = "Full";
         $displayStyle = 'style="display:none;"';
      }
   }
   
   $streamButton = "MJPEG-Stream";
   $mjpegmode = 0;
   if(isset($_COOKIE["stream_mode"])) {
      if($_COOKIE["stream_mode"] == "MJPEG-Stream") {
         $streamButton = "Default-Stream";
         $mjpegmode = 1;
      }
   }
   $config = readConfig($config, CONFIG_FILE1);
   $config = readConfig($config, CONFIG_FILE2);
   $video_fps = $config['video_fps'];
   $divider = $config['divider'];
  ?>

<html>
    <head>
        <meta name="viewport" content="width=550, initial-scale=1">
        <title>StarPi</title>
        <!-- Bootstrap core CSS -->
        <link rel="stylesheet" href="css/bootstrap.min.css">
        
      <link rel="stylesheet" href="css/style_minified.css" />
        <link rel="stylesheet" href="<?php echo getStyle(); ?>" />
        <script src="js/bootstrap.min.js"></script>
        <script src="js/jquery.min.js"></script>
        <script src="js/style_minified.js"></script>
        <script src="js/script.js"></script>
        <script src="js/pipan.js"></script>
        <script src="js/StarPi.js"></script>
     </head>
     
     <body onload="setTimeout('init(<?php echo "$mjpegmode, $video_fps, $divider" ?>);', 100);">
        
        <div class="container-fluid text-center liveimage" >
            <div><img id="mjpeg_dest" <?php echo getImgWidth();?> <?php if(file_exists("pipan_on")) echo "ontouchstart=\"pipan_start()\""; ?> onclick="toggle_fullscreen(this);" src="./loading.jpg"></div>
            <div id="main-buttons" <?php echo $displayStyle; ?> >
                <input id="video_button" type="button" class="btn btn-primary">
                <input id="image_button" type="button" class="btn btn-primary">
                <input id="timelapse_button" type="button" class="btn btn-primary">
                <input id="halt_button" type="button" class="btn btn-danger">
                <input id="download_button" type="button" class="btn btn-primary" onclick="location.href='preview.php';" value="Download Videos and Images">
            </div>
        </div>
        
        <ul class="nav nav-pills nav-justified" >
            <li class="dropdown">
                <a class="dropdown-toggle active" data-toggle="dropdown" href="#Telescope">Telescope
                <span class="caret"></span></a>
                <ul class="dropdown-menu">
                    <li><a data-toggle="pill" href="#TelescopeHorizontalCoordinates">Horizontal Coordinates</a></li>
                    <li><a data-toggle="pill" href="#TelescopeEquitorialCoordinates">Equitorial Coordinates</a></li>
                    <li><a data-toggle="pill" href="#TelescopeConfiguration">Configuration</a></li> 
                </ul>
            </li>
            <li class="dropdown">
                <a class="dropdown-toggle" data-toggle="dropdown" href="#Camera">Camera
                <span class="caret"></span></a>
                <ul class="dropdown-menu">
                    <li><a data-toggle="pill" href="#CameraPicture">Picture</a></li>
                    <li><a data-toggle="pill" href="#CameraVideo">Video</a></li> 
                    <li><a data-toggle="pill" href="#CameraResolution">Resolution</a></li>
                    <li><a data-toggle="pill" href="#CameraQuality">Quality</a></li>
                    <li><a data-toggle="pill" href="#CameraEffect">Effect</a></li>
                    <li><a data-toggle="pill" href="#CameraConfiguration">Configuration</a></li> 
                </ul>
            </li>
            <li class="dropdown">
                <a class="dropdown-toggle" data-toggle="dropdown" href="#Location">Location
                <span class="caret"></span></a>
                <ul class="dropdown-menu">
                    <li><a data-toggle="pill" href="#LocationLocation">Location</a></li>
                    <li><a data-toggle="pill" href="#LocationMagnetics">Magnetic Information</a></li>
                </ul>
            </li>
            <li><a data-toggle="pill" href="#TimeandDate">Time and Date</a></li>
            <li><a data-toggle="pill" href="#System">System</a></li>
        </ul>
        
        
        <div id="telescope-content" class="container-fluid text-center" <?php echo $displayStyle; ?> >
        <div class="tab-content">
            <div id="Home" class="tab-pane fade in active">
                <h3>Home</h3>
                <p>Some content.</p>
            </div>
            <div id="TelescopeHorizontalCoordinates" class="tab-pane fade">
                <h3>Horizontal Coordinates</h3>
                <table align="center">
                   <tr>
                        <td align="left">Altitude:   </td>
                        <td id="Azimuth" align="left">0</td>
                    </tr>
                    <tr>
                        <td align="left">Azimuth:    </td>
                        <td id="Altitude" align="left">0</td>
                    </tr>
                </table>
            </div>
            <div id="TelescopeEquitorialCoordinates" class="tab-pane fade">
                <h3>Equitorial Coordinates</h3>
                <table align="center">
                    <tr>
                        <td align="left">Declination:</td>
                        <td align="left"><span  id="declination">0</span>&deg <span id="declinationminutes">0</span>'<span id="declinationseconds">0</span>"</td>
                    </tr>
                    <tr>
                        <td align="left">Right Ascension:   </td>
                        <td align="left"><span  id="right_ascension">0</span >h<span id="raminutes">0</span>m<span id="raseconds">0</span>s</td>
                    </tr>
                </table>
            </div>
            <div id="TelescopeConfiguration" class="tab-pane fade">
                <h3>Telescope Configuration</h3>
                <table align="center">
                    <tr>
                        <td align="left">status:   </td>
                        <td id="status" align="left">Disconnect</td>
                    </tr>
                </table>
            </div>
            <div id="CameraResolution" class="tab-pane fade">
                <h3>Resolution</h3>
                <table align="center">
                    <tr>
                        <td align="left">Load Preset:</td>
                        <td align="left">
                            <select onchange="set_preset(this.value)">
                                <option value="1920 1080 25 25 2592 1944">Select option...</option>
                                <option value="1920 1080 25 25 2592 1944">Full HD 1080p 16:9</option>
                                <option value="1280 0720 25 25 2592 1944">HD-ready 720p 16:9</option>
                                <option value="1296 972 25 25 2592 1944">Max View 972p 4:3</option>
                                <option value="768 576 25 25 2592 1944">SD TV 576p 4:3</option>
                                <option value="1920 1080 01 30 2592 1944">Full HD Timelapse (x30) 1080p 16:9</option>
                            </select>
                        </td>
                    </tr>
                    <tr>
                        <td align="left">Custom Values:</td>
                    </tr>
                    <tr>
                        <td align="left">Video res: </td>
                        <td align="left"><?php makeInput('video_width', 4); ?>x<?php makeInput('video_height', 4); ?>px</td>
                    </tr>
                    <tr>
                        <td align="left">Video fps:</td>
                        <td align="left"><?php makeInput('video_fps', 2); ?>recording, <?php makeInput('MP4Box_fps', 2); ?>boxing</td>
                    </tr>
                    <tr>
                        <td align="left">Image res:</td>
                        <td align="left"><?php makeInput('image_width', 4); ?>x<?php makeInput('image_height', 4); ?>px<td>
                    </tr>
                    <tr>
                        <td align="left"><input type="button" value="OK" onclick="set_res();"></td>
                    </tr>
                </table>
            </div>
            <div id="CameraPicture" class="tab-pane fade">
                <h3>Picture</h3>
                <table align="center">        
                    <tr>
                        <td align="left">Timelapse-Interval (0.1...3200):</td>
                        <td align="left"><?php makeInput('tl_interval', 4); ?>s <input type="button" value="OK" onclick="send_cmd('tv ' + 10 * document.getElementById('tl_interval').value)"></td>
                    </tr>
                    <tr>
                        <td align="left">Annotation (max 127 characters):</td>
                        <td align="left">
                        Text: <?php makeInput('annotation', 20); ?><input type="button" value="OK" onclick="send_cmd('an ' + encodeURI(document.getElementById('annotation').value))"><input type="button" value="Default" onclick="document.getElementById('annotation').value = 'RPi Cam %Y.%M.%D_%h:%m:%s'; send_cmd('an ' + encodeURI(document.getElementById('annotation').value))"><br>
                        Background: ><select onchange="send_cmd('ab ' + this.value)"><?php makeOptions($options_ab, 'anno_background'); ?></select>
                        </td>
                    </tr>
                    <?php if (file_exists("pilight_on")) pilight_controls(); ?>
                    </tr>
                </table>
            </div>
            <div id="CameraEffect" class="tab-pane fade">
                <h3>Effects</h3>
                <table  align="center">
                    <tr>
                        <td align="left">Rotation, default 0:</td>
                        <td align="left"><select onchange="send_cmd('ro ' + this.value)"><?php makeOptions($options_ro, 'rotation'); ?></select></td>
                    </tr>
                    <tr>
                        <td align="left">Flip, default 'none':</td>
                        <td align="left"><select onchange="send_cmd('fl ' + this.value)"><?php makeOptions($options_fl, 'flip'); ?></select></td>
                    </tr>
                    <tr>
                        <td align="left">Image Effect, default 'none':</td>
                        <td align="left"><select onchange="send_cmd('ie ' + this.value)"><?php makeOptions($options_ie, 'image_effect'); ?></select></td>
                    </tr>
                    <tr>
                        <td align="left">Colour Effect, default 'disabled':</td>
                        <td align="left"><select id="ce_en"><?php makeOptions($options_ce_en, 'colour_effect_en'); ?></select>
                        y:u:v = <?php makeInput('ce_u', 4, 'colour_effect_u'); ?>:<?php makeInput('ce_v', 4, 'colour_effect_v'); ?>
                        <input type="button" value="OK" onclick="set_ce();">
                        </td>
                    </tr>
                    <tr>
                        <td align="left">Custom text color:</td>
                        <td align="left"><select id="at_en"><?php makeOptions($options_at_en, 'anno3_custom_text_colour'); ?></select>
                            y:u:v = <?php makeInput('at_y', 3, 'anno3_custom_text_Y'); ?>:<?php makeInput('at_u', 4, 'anno3_custom_text_U'); ?>:<?php makeInput('at_v', 4, 'anno3_custom_text_V'); ?>
                            <input type="button" value="OK" onclick="set_at();">
                        </td>
                    </tr>
                    <tr>
                        <td align="left">Custom background color:</td>
                        <td align="left"><select id="ac_en"><?php makeOptions($options_ac_en, 'anno3_custom_background_colour'); ?></select>
                            y:u:v = <?php makeInput('ac_y', 3, 'anno3_custom_background_Y'); ?>:<?php makeInput('ac_u', 4, 'anno3_custom_background_U'); ?>:<?php makeInput('ac_v', 4, 'anno3_custom_background_V'); ?>
                            <input type="button" value="OK" onclick="set_ac();">
                        </td>
                    </tr>
                </table>
            </div>
            <div id="CameraQuality" class="tab-pane fade">
                <h3>Quality</h3>
                <table align="center">
                    <tr>
                        <td align="left">Sharpness (-100...100), default 0:</td>
                        <td align="left"><?php makeInput('sharpness', 4); ?><input type="button" value="OK" onclick="send_cmd('sh ' + document.getElementById('sharpness').value)"></td>
                    </tr>
                    <tr>
                        <td align="left">Contrast (-100...100), default 0:</td>
                        <td align="left"><?php makeInput('contrast', 4); ?><input type="button" value="OK" onclick="send_cmd('co ' + document.getElementById('contrast').value)">
                        </td>
                    </tr>
                    <tr>
                        <td align="left">Brightness (0...100), default 50:</td>
                        <td align="left"><?php makeInput('brightness', 4); ?><input type="button" value="OK" onclick="send_cmd('br ' + document.getElementById('brightness').value)"></td>
                    </tr>
                    <tr>
                        <td align="left">Saturation (-100...100), default 0:</td>
                        <td align="left"><?php makeInput('saturation', 4); ?><input type="button" value="OK" onclick="send_cmd('sa ' + document.getElementById('saturation').value)"></td>
                    </tr>
                    <tr>
                        <td align="left">ISO (100...800), default 0:</td>
                        <td align="left"><?php makeInput('iso', 4); ?><input type="button" value="OK" onclick="send_cmd('is ' + document.getElementById('iso').value)"></td>
                    </tr>
                    <tr>
                        <td align="left">Metering Mode, default 'average':</td>
                        <td align="left"><select onchange="send_cmd('mm ' + this.value)"><?php makeOptions($options_mm, 'metering_mode'); ?></select></td>
                    </tr>
                    <tr>
                        <td align="left">Exposure Compensation (-10...10), default 0:</td>
                        <td align="left"><?php makeInput('exposure_compensation', 4); ?><input type="button" value="OK" onclick="send_cmd('ec ' + document.getElementById('exposure_compensation').value)"></td>
                    </tr>
                    <tr>
                        <td align="left">Exposure Mode, default 'auto':</td>
                        <td align="left"><select onchange="send_cmd('em ' + this.value)"><?php makeOptions($options_em, 'exposure_mode'); ?></select></td>
                    </tr>
                    <tr>
                        <td align="left">White Balance, default 'auto':</td>
                        <td><select onchange="send_cmd('wb ' + this.value)"><?php makeOptions($options_wb, 'white_balance'); ?></select></td>
                    </tr>
                </table>
            </div>
                <div id="CameraConfiguration" class="tab-pane fade">
                <h3>Camera Configuration</h3>
                <table align="center">
                    
                    <?php  if($config['camera_num'] > 0): ?> 
                    <tr>
                        <td align="left">Camera select (Compute module only)</td>
                        <td align="left">
                        Use camera: <select onchange="send_cmd('cn ' + this.value)"><?php makeOptions($options_cn, 'camera_num'); ?></select>
                    </td>
                    </tr>
                    <?php endif; ?>
                    <tr>
                        <td align="left">Buffer (1000... ms), default 0:</td>
                        <td align="left"><?php makeInput('video_buffer', 4); ?><input type="button" value="OK" onclick="send_cmd('bu ' + document.getElementById('video_buffer').value)"></td>
                    </tr>
                    <tr>
                        <td align="left">Image Statistics, default 'Off':</td>
                        <td align="left"><select onchange="send_cmd('st ' + this.value)"><?php makeOptions($options_st, 'stat_pass'); ?></select></td>
                    </tr>
                    <tr>
                        <td align="left">Sensor Region, default 0/0/65536/65536:</td>
                        <td align="left">
                        x<?php makeInput('roi_x', 5, 'sensor_region_x'); ?> y<?php makeInput('roi_y', 5, 'sensor_region_y'); ?> w<?php makeInput('roi_w', 5, 'sensor_region_w'); ?> h<?php makeInput('roi_h', 4, 'sensor_region_h'); ?> <input type="button" value="OK" onclick="set_roi();">
                        </td>
                    </tr>
                    <tr>
                        <td align="left">Shutter speed (0...330000), default 0:</td>
                        <td align="left"><?php makeInput('shutter_speed', 4); ?><input type="button" value="OK" onclick="send_cmd('ss ' + document.getElementById('shutter_speed').value)">
                        </td>
                    </tr>
                    <tr>
                        <td align="left">Image quality (0...100), default 85:</td>
                        <td align="left">
                        <?php makeInput('image_quality', 4); ?><input type="button" value="OK" onclick="send_cmd('qu ' + document.getElementById('image_quality').value)">
                        </td>
                    </tr>
                    <tr>
                        <td align="left">Preview quality (0...100) Default 25:<br>Width (128...1024) Default 512:<br>Divider (1-16) Default 1:</td>
                        <td align="left">
                            Qu: <?php makeInput('quality', 4); ?>
                            Wi: <?php makeInput('width', 4); ?>
                            Di: <?php makeInput('divider', 4); ?>
                            <input type="button" value="OK" onclick="set_preview();">
                        </td>
                    </tr>
                    <tr>
                        <td align="left">Raw Layer, default: 'off'</td>
                        <td align="left"><select onchange="send_cmd('rl ' + this.value)"><?php makeOptions($options_rl, 'raw_layer'); ?></select></td>
                    </tr>
                </table>
        
            </div>
            <div id="CameraVideo" class="tab-pane fade">
                <h3>Video Settings</h3>        
                <table align="center">
                     <tr>
                        <td align="left">Video Stabilisation, default: 'off'</td>
                        <td align="left"><select onchange="send_cmd('vs ' + this.value)"><?php makeOptions($options_vs, 'video_stabilisation'); ?></select></td>
                    </tr>
                   <tr>
                        <td align="left">Video bitrate (0...25000000), default 17000000:</td>
                        <td align="left">
                        <?php makeInput('video_bitrate', 10); ?><input type="button" value="OK" onclick="send_cmd('bi ' + document.getElementById('video_bitrate').value)">
                        </td>
                    </tr>
                    <tr>
                        <td align="left">MP4 Boxing mode :</td>
                        <td align="left"><select onchange="send_cmd('bo ' + this.value)"><?php makeOptions($options_bo, 'MP4Box'); ?></select></td>
                    </tr>
                    <tr>
                        <td align="left">Annotation size(0-99):</td>
                        <td align="left">
                        <?php makeInput('anno_text_size', 3); ?><input type="button" value="OK" onclick="send_cmd('as ' + document.getElementById('anno_text_size').value)">
                        </td>
                    </tr>
                    <tr>
                        <td align="left">Watchdog, default interval 3s, errors 3</td>
                        <td align="left">Interval <?php makeInput('watchdog_interval', 3); ?>s&nbsp;&nbsp;&nbsp;&nbsp;Errors <?php makeInput('watchdog_errors', 3); ?>
                        <input type="button" value="OK" onclick="send_cmd('wd ' + 10 * document.getElementById('watchdog_interval').value + ' ' + document.getElementById('watchdog_errors').value)">
                        </td>
                    </tr>                    
                </table>
            </div>
            <div id="LocationLocation" class="tab-pane fade">
                <h3>Location</h3>
                <table align="center">
                    <tr>
                        <td align="left">Latitude:</td>
                        <td id="Latitude" align="left">0</td>
                        <td align="left"><input type="text" Id="UserLatitude" value="0"><input type="button" value="Ok" onclick="sendLatitude();"></td>
                    </tr>
                    <tr>
                        <td align="left">longitude:</td>
                        <td id="longitude" align="left">0</td>
                        <td align="left"><input type="text" Id="UserLongitude" value="0"><input type="button" value="Ok" onclick="sendLongitude()"></td>
                    </tr>
                    <tr>
                        <td align="left">Height:</td>
                        <td id="Height" align="left">0</td>
                        <td align="left"><input type="text" Id="UserHeight" value="0"><input type="button" value="Ok" onclick="sendHeight()"></td>
                    </tr>
                    <tr>
                        <td align="left">Location Data Source</td>
                        <td align="left"><button type="button" onclick="sendSetUser()">User</button><button type="button" onclick="sendSetGps()">GPS</button></td>
                    </tr>
                </table>
            </div>
            <div id="LocationMagnetics" class="tab-pane fade">
                <h3>Magnetic Information</h3>
                <table align="center">
                    <tr>
                        <td align="left">Magnetic Declination:</td>
                        <td id="MagneticDeclination" align="left">0</td>
                    </tr>
                    <tr>
                        <td align="left">True Heading:</td>
                        <td id="TrueHeading" align="left">0</td>
                    </tr>
                    <tr>
                        <td align="left">Magnetic Heading:</td>
                        <td id="MagneticHeading" align="left">0</td>
                    </tr>
                </table>
            </div>
            <div id="TimeandDate" class="tab-pane fade">
                <h3>Time and Date</h3>
                <table  align="center">
                    <tr>
                        <td align="left">Date:</td>
                        <td align="left"><span  id="Day">0</span >:<span id="Month">0</span>:<span id="Year">0</span></td>
                    </tr>
                    <tr>
                        <td align="left">Time:</td>
                        <td align="left"><span  id="TimeDisplay">0</span >h<span id="gmtminutes">0</span>m<span id="gmtseconds">0</span>s</td>
                    </tr>
                    <tr>
                        <td align="left">SummerTime:</td>
                        <td id="SummerTime">0</td>
                    </tr>
                    <tr>
                        <td align="left">Local Sidreal Time:</td>
                        <td align="left"><span  id="lst">0</span >h<span id="localsiderealminutes">0</span>m<span id="localsiderealseconds">0</span>s</td>
                    </tr>
                    <tr>
                        <td align="left"> Julian Date:</td>
                        <td id="juliandate" align="left">0</td>
                    </tr>
                </table>
            </div>
            <div id="System" class="tab-pane fade">
                <h3>System Control</h3>
                <input id="toggle_stream" type="button" class="btn btn-primary" value="<?php echo $streamButton; ?>" onclick="set_stream_mode(this.value);">
                <input id="shutdown_button" type="button" value="shutdown system" onclick="sys_shutdown();" class="btn btn-danger">
                <input id="reboot_button" type="button" value="reboot system" onclick="sys_reboot();" class="btn btn-danger">
                <input id="reset_button" type="button" value="reset settings" onclick="send_cmd('rs 1');setTimeout(function(){location.reload(true);}, 1000);" class="btn btn-danger">
            </div>
        </div>        
        <?php if ($debugString != "") echo "$debugString<br>"; ?>
    </body>
</html>
