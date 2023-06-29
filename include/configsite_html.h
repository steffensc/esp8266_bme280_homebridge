String configsite_config_html =
"<!DOCTYPE HTML>"
"<html>"
"<head>"
"<meta name='viewport' content='width=device-width, initial-scale=1.0'>"
"<style type='text/css'>"
"body{font-family:sans-serif; max-width:500px; margin:auto;padding:0px 15px;background-color:#111;color:#fff;}"
"li{padding:10px 0px;}"
"h1{text-align:center;}"
"form input{font-size:12pt; display:block; width:100%%; border:1px solid #fff; padding:5px 0px; margin-bottom:10px;}"
"</style>"
"</head>"
"<body>"
"<h1>%s<br>%s</h1>"
"<br>"
"<h3>Current top 5 WiFi-Networks in reach:</h3>"
"<ol>%s</ol>"
"<br>"
"<h3>Configure new WiFi:</h3>"
"<form method='get' action='setting' style='width:100%;'>"
"<label>SSID:</label>"
"<input id='ssid' name='ssid' length=31>"
"<br>"
"<label>PASS:</label>"
"<input name='pass' length=63>"
"<br>"
"<label>SENSOR NO.:</label>"
"<input name='sensorno' length=2>"
"<br><br>"
"<input type='submit'>"
"</form>"
"<br><br>"
"<form method='get' action='rescan' style='width:100%;'>"
"<input value='Rescan WiFi Networks' type='submit'>"
"</form>"
"</body>"
"<script>"
"var elem = document.getElementsByTagName('li');"
"for (n = 0; n < elem.length; ++n){"
"elem[n].onclick = clickHandler;"
"}"
"function clickHandler(){"
"document.getElementById('ssid').value = this.id;"
"}"
"</script>"
"</html>";

String configsite_success_html =
"<!DOCTYPE HTML>"
"<html>"
"<head>"
"<style type='text/css'>"
"body{font-family: sans-serif;}"
"</style>"
"</head>"
"<body>"
"<h1>Success!</h1>"
"<br>"
"Data saved to EEPROM."
"<br><br>"
"...resetting for boot into new WiFi."
"</body>"
"</html>";

String configsite_error_html =
"<!DOCTYPE HTML>"
"<html>"
"<head>"
"<style type='text/css'>"
"body{font-family: sans-serif;}"
"</style>"
"</head>"
"<body>"
"<h1>ERROR!</h1>"
"<br>"
"Data could not be saved to EEPROM."
"</body>"
"</html>";