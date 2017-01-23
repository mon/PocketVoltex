#include <GuiListView.au3>

Func SelectLight($list, $num, $name)
   _GUICtrlListView_ClickItem($list, $num, "primary", False, 2)
   WinWait("Light Binding")
   $bind = WinGetHandle("Light Binding")
   ControlCommand($bind,"", "ComboBox1","SelectString",$name)
   ControlClick($bind,"", "Button1")
EndFunc

Local $lights[8] = [ _
   "Wing Right Up", "Wing Right Up", "Wing Left Low", "Wing Right Low", _
   "Woofer", "Woofer", "Controller", "Controller" _
]

Local $colours[3] = ["B", "G", "R"]
Local $bt[6] = ["Button A", "Button B", "Button C", "Button D", "FX-L", "FX-R"]

WinWait("BemaniPC Input Configuration")
$win = WinGetHandle("BemaniPC Input Configuration")

WinActivate($win)
ControlClick($win, "", "[CLASS:SysTabControl32; INSTANCE:1]")
Send("{LEFT}{LEFT}{LEFT}{RIGHT}")
ControlCommand ($win,"", "ComboBox1","SelectString","Pocket Voltex LEDs")

$list = ControlGetHandle($win, "", "SysListView321")
For $i = 0 To UBound($lights) - 1
   For $j = 0 to UBound($colours) - 1
	  SelectLight($list, $i * UBound($colours) + $j, $lights[$i] & " " & $colours[$j])
	Next
Next

For $i = 0 To UBound($bt) - 1
   SelectLight($list, $i + UBound($lights)*UBound($colours), $bt[$i])
Next