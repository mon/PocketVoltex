(function(window, document) {
"use strict";

var vid = 0x16D0;
var pid = 0x0A6D;
var UsbWrapper = window.UsbWrapper;

var device;

var SWITCH_COUNT = 8;
var CONFIG_OUT_EPADDR = 1;
var CONFIG_IN_EPADDR  = 2;
var packetSize = 64;

var commands = {
    GETCONFIG : 1,
    SETCONFIG : 2,
    VERSION   : 3,
    RESET :     42
};

var defaultConfig = {
    switches   : [],
    ledsOn     : true,
    //macroClick : KP_PLUS,
    macroPin   : [1,2,3,4]
}

var visibleLog = function(html) {
    console.log(html);
    document.getElementById('logview').innerHTML += html + '<br/>';
}

var ConfigValues = function(view) {
    // skip the returned command byte
    var offset = 1;
    
    this.switches = this.getKeys(view, offset, SWITCH_COUNT);
    offset += SWITCH_COUNT;
    this.btColour = this.getColour(view, offset);
    offset += 3;
    this.fxColour = this.getColour(view, offset);
    offset += 3;
    this.knobColours = [];
    for(var i = 0; i < 2; i++) {
        this.knobColours[i] = this.getColour(view, offset);
        offset += 3;
    }
    
    this.lightsOn = !!(view.getUint8(offset++));
    this.hidLights = !!(view.getUint8(offset++));
    this.keyLights = !!(view.getUint8(offset++));
    this.knobLights = !!(view.getUint8(offset++));
    this.lightPattern = view.getUint8(offset++);
    this.macroClick = this.getKey(view, offset++);
    this.macroPin = this.getArray(view, offset, 4);
    offset += 4;
    this.joystickMode = !!(view.getUint8(offset++));
}

ConfigValues.prototype.getArray = function(view, offset, size) {
    var ret = [];
    for(var i = 0; i < size; i++) {
        ret[i] = view.getUint8(offset+i);
    }
    return ret;
};

ConfigValues.prototype.getColour = function(view, offset) {
    return 'rgb(' + this.getArray(view, offset, 3).join() + ')';
};

ConfigValues.prototype.getKeys = function(view, offset, size) {
    var keys = [];
    for(let i = 0; i < size; i++) {
        keys[i] = this.getKey(view, offset++);
    }
    return keys;
}

ConfigValues.prototype.getKey = function(view, offset) {
    var key = view.getUint8(offset);
    return scancodes.find(code => {return code.value == key});
}

var Config = new function() {
    this.connect = () => {
        return UsbWrapper.connect(vid, pid)
        .then(selectedDevice => {
            device = selectedDevice;
            visibleLog("Opening device...");
            return device.open();
        })
        .then(() => {
            console.log("Opened, selecting configuration...");
            return device.selectConfiguration(1)
        })
        .then(() => {
            console.log("Selected, claiming interface...");
            return device.claimInterface(0);
        })
        .then(() => {
            visibleLog("Opened!");
            return this.readVersion();
        })
        .then(version => {
            console.log("Found Pocket Voltex v" + version/10.0);
            return this.readConfig();
        })
        .then(result => {console.log(result);})
        .then( () => {
            if(device && device.opened)
                device.close();
        })
        .catch(error => {
            //alert(error);
            visibleLog(error);
            if(device && device.opened)
                device.close();
        });
    };
    
    this.readVersion = () => {
        if(!device || !device.opened)
            return Promise.reject("Device not opened");
        
        var data = new Uint8Array(packetSize);
        data[0] = commands.VERSION;
        return device.transferOut(CONFIG_OUT_EPADDR, data)
        .then(() => device.transferIn(CONFIG_IN_EPADDR, packetSize))
        .then(result => {
            console.log(result);
            console.log("Got version response of len", result.data.buffer.byteLength);
            // version int exists at offset 1
            return result.data.getUint16(1, true);
        })
    };
    
    this.readConfig = () => {
        if(!device || !device.opened)
            return Promise.reject("Device not opened");
        var data = new Uint8Array(packetSize);
        data[0] = commands.GETCONFIG;
        return device.transferOut(CONFIG_OUT_EPADDR, data)
        .then(() => device.transferIn(CONFIG_IN_EPADDR, packetSize))
        .then(result => {
            console.log("Got config response of len", result.data.buffer.byteLength);
            return new ConfigValues(result.data);
        });
    };
    
    this.close = () => {
        if(!device || !device.opened)
            return Promise.reject("Device not opened");
        visibleLog("Closing device...");
        return device.close();
    };
    
    this.rebootToBootloader = () => {
        if(!device || !device.opened)
            return Promise.reject("Device not opened");
        var data = new Uint8Array(packetSize);
        data[0] = commands.RESET;
        return device.transferOut(1, data);
    };
}

window.Config = Config;

var scancodes = [
  {name: "A", value: 0x04},
  {name: "B", value: 0x05},
  {name: "C", value: 0x06},
  {name: "D", value: 0x07},
  {name: "E", value: 0x08},
  {name: "F", value: 0x09},
  {name: "G", value: 0x0A},
  {name: "H", value: 0x0B},
  {name: "I", value: 0x0C},
  {name: "J", value: 0x0D},
  {name: "K", value: 0x0E},
  {name: "L", value: 0x0F},
  {name: "M", value: 0x10},
  {name: "N", value: 0x11},
  {name: "O", value: 0x12},
  {name: "P", value: 0x13},
  {name: "Q", value: 0x14},
  {name: "R", value: 0x15},
  {name: "S", value: 0x16},
  {name: "T", value: 0x17},
  {name: "U", value: 0x18},
  {name: "V", value: 0x19},
  {name: "W", value: 0x1A},
  {name: "X", value: 0x1B},
  {name: "Y", value: 0x1C},
  {name: "Z", value: 0x1D},
  {name: "1", value: 0x1E},
  {name: "2", value: 0x1F},
  {name: "3", value: 0x20},
  {name: "4", value: 0x21},
  {name: "5", value: 0x22},
  {name: "6", value: 0x23},
  {name: "7", value: 0x24},
  {name: "8", value: 0x25},
  {name: "9", value: 0x26},
  {name: "0", value: 0x27},
  {name: "-_", value: 0x2D},
  {name: "=+", value: 0x2E},
  {name: "[{", value: 0x2F},
  {name: "]}", value: 0x30},
  {name: "\\|", value: 0x31},
  {name: ";:", value: 0x33},
  {name: "'\"", value: 0x34},
  {name: "`~", value: 0x35},
  {name: ",<", value: 0x36},
  {name: ".>", value: 0x37},
  {name: "/?", value: 0x38},
  {name: "F1", value: 0x3A},
  {name: "F2", value: 0x3B},
  {name: "F3", value: 0x3C},
  {name: "F4", value: 0x3D},
  {name: "F5", value: 0x3E},
  {name: "F6", value: 0x3F},
  {name: "F7", value: 0x40},
  {name: "F8", value: 0x41},
  {name: "F9", value: 0x42},
  {name: "F10", value: 0x43},
  {name: "F11", value: 0x44},
  {name: "F12", value: 0x45},
  {name: "ESC", value: 0x29},
  {name: "BACKSPC", value: 0x2A},
  {name: "TAB", value: 0x2B},
  {name: "SPACE", value: 0x2C},
  {name: "ENTER", value: 0x28},
  {name: "CAPSLOCK", value: 0x39},
  {name: "PRTSCRN", value: 0x46},
  {name: "SCRL LCK", value: 0x47},
  {name: "PAUSE", value: 0x48},
  {name: "INSERT", value: 0x49},
  {name: "HOME", value: 0x4A},
  {name: "PG UP", value: 0x4B},
  {name: "DEL", value: 0x4C},
  {name: "END", value: 0x4D},
  {name: "PG DN", value: 0x4E},
  {name: "RIGHT", value: 0x4F},
  {name: "LEFT", value: 0x50},
  {name: "DOWN", value: 0x51},
  {name: "UP", value: 0x52},
  {name: "NUM LOCK", value: 0x53},
  {name: "NUM /", value: 0x54},
  {name: "NUM *", value: 0x55},
  {name: "NUM -", value: 0x56},
  {name: "NUM +", value: 0x57},
  {name: "NUM ENTER", value: 0x58},
  {name: "NUM 1", value: 0x59},
  {name: "NUM 2", value: 0x5A},
  {name: "NUM 3", value: 0x5B},
  {name: "NUM 4", value: 0x5C},
  {name: "NUM 5", value: 0x5D},
  {name: "NUM 6", value: 0x5E},
  {name: "NUM 7", value: 0x5F},
  {name: "NUM 8", value: 0x60},
  {name: "NUM 9", value: 0x61},
  {name: "NUM 0", value: 0x62},
  {name: "NUM .", value: 0x63}
];

//{name: "NON US BACKSLASH AND PIPE", value: 0x64},
//{name: "APPLICATION", value: 0x65},
//{name: "POWER", value: 0x66},
//{name: "KEYPAD EQUAL SIGN", value: 0x67},
//{name: "F13", value: 0x68},
//{name: "F14", value: 0x69},
//{name: "F15", value: 0x6A},
//{name: "F16", value: 0x6B},
//{name: "F17", value: 0x6C},
//{name: "F18", value: 0x6D},
//{name: "F19", value: 0x6E},
//{name: "F20", value: 0x6F},
//{name: "F21", value: 0x70},
//{name: "F22", value: 0x71},
//{name: "F23", value: 0x72},
//{name: "F24", value: 0x73},
//{name: "EXECUTE", value: 0x74},
//{name: "HELP", value: 0x75},
//{name: "MENU", value: 0x76},
//{name: "SELECT", value: 0x77},
//{name: "STOP", value: 0x78},
//{name: "AGAIN", value: 0x79},
//{name: "UNDO", value: 0x7A},
//{name: "CUT", value: 0x7B},
//{name: "COPY", value: 0x7C},
//{name: "PASTE", value: 0x7D},
//{name: "FIND", value: 0x7E},
//{name: "MUTE", value: 0x7F},
//{name: "VOLUME UP", value: 0x80},
//{name: "VOLUME DOWN", value: 0x81},
//{name: "LOCKING CAPS LOCK", value: 0x82},
//{name: "LOCKING NUM LOCK", value: 0x83},
//{name: "LOCKING SCROLL LOCK", value: 0x84},
//{name: "KEYPAD COMMA", value: 0x85},
//{name: "KEYPAD EQUAL SIGN AS400", value: 0x86},
//{name: "INTERNATIONAL1", value: 0x87},
//{name: "INTERNATIONAL2", value: 0x88},
//{name: "INTERNATIONAL3", value: 0x89},
//{name: "INTERNATIONAL4", value: 0x8A},
//{name: "INTERNATIONAL5", value: 0x8B},
//{name: "INTERNATIONAL6", value: 0x8C},
//{name: "INTERNATIONAL7", value: 0x8D},
//{name: "INTERNATIONAL8", value: 0x8E},
//{name: "INTERNATIONAL9", value: 0x8F},
//{name: "LANG1", value: 0x90},
//{name: "LANG2", value: 0x91},
//{name: "LANG3", value: 0x92},
//{name: "LANG4", value: 0x93},
//{name: "LANG5", value: 0x94},
//{name: "LANG6", value: 0x95},
//{name: "LANG7", value: 0x96},
//{name: "LANG8", value: 0x97},
//{name: "LANG9", value: 0x98},
//{name: "ALTERNATE ERASE", value: 0x99},
//{name: "SYSREQ", value: 0x9A},
//{name: "CANCEL", value: 0x9B},
//{name: "CLEAR", value: 0x9C},
//{name: "PRIOR", value: 0x9D},
//{name: "RETURN", value: 0x9E},
//{name: "SEPARATOR", value: 0x9F},
//{name: "OUT", value: 0xA0},
//{name: "OPER", value: 0xA1},
//{name: "CLEAR AND AGAIN", value: 0xA2},
//{name: "CRSEL AND PROPS", value: 0xA3},
//{name: "EXSEL", value: 0xA4},
//{name: "KEYPAD 00", value: 0xB0},
//{name: "KEYPAD 000", value: 0xB1},
//{name: "THOUSANDS SEPARATOR", value: 0xB2},
//{name: "DECIMAL SEPARATOR", value: 0xB3},
//{name: "CURRENCY UNIT", value: 0xB4},
//{name: "CURRENCY SUB UNIT", value: 0xB5},
//{name: "KEYPAD OPENING PARENTHESIS", value: 0xB6},
//{name: "KEYPAD CLOSING PARENTHESIS", value: 0xB7},
//{name: "KEYPAD OPENING BRACE", value: 0xB8},
//{name: "KEYPAD CLOSING BRACE", value: 0xB9},
//{name: "KEYPAD TAB", value: 0xBA},
//{name: "KEYPAD BACKSPACE", value: 0xBB},
//{name: "KEYPAD A", value: 0xBC},
//{name: "KEYPAD B", value: 0xBD},
//{name: "KEYPAD C", value: 0xBE},
//{name: "KEYPAD D", value: 0xBF},
//{name: "KEYPAD E", value: 0xC0},
//{name: "KEYPAD F", value: 0xC1},
//{name: "KEYPAD XOR", value: 0xC2},
//{name: "KEYPAD CARET", value: 0xC3},
//{name: "KEYPAD PERCENTAGE", value: 0xC4},
//{name: "KEYPAD LESS THAN SIGN", value: 0xC5},
//{name: "KEYPAD GREATER THAN SIGN", value: 0xC6},
//{name: "KEYPAD AMP", value: 0xC7},
//{name: "KEYPAD AMP AMP", value: 0xC8},
//{name: "KEYPAD PIPE", value: 0xC9},
//{name: "KEYPAD PIPE PIPE", value: 0xCA},
//{name: "KEYPAD COLON", value: 0xCB},
//{name: "KEYPAD HASHMARK", value: 0xCC},
//{name: "KEYPAD SPACE", value: 0xCD},
//{name: "KEYPAD AT", value: 0xCE},
//{name: "KEYPAD EXCLAMATION SIGN", value: 0xCF},
//{name: "KEYPAD MEMORY STORE", value: 0xD0},
//{name: "KEYPAD MEMORY RECALL", value: 0xD1},
//{name: "KEYPAD MEMORY CLEAR", value: 0xD2},
//{name: "KEYPAD MEMORY ADD", value: 0xD3},
//{name: "KEYPAD MEMORY SUBTRACT", value: 0xD4},
//{name: "KEYPAD MEMORY MULTIPLY", value: 0xD5},
//{name: "KEYPAD MEMORY DIVIDE", value: 0xD6},
//{name: "KEYPAD PLUS AND MINUS", value: 0xD7},
//{name: "KEYPAD CLEAR", value: 0xD8},
//{name: "KEYPAD CLEAR ENTRY", value: 0xD9},
//{name: "KEYPAD BINARY", value: 0xDA},
//{name: "KEYPAD OCTAL", value: 0xDB},
//{name: "KEYPAD DECIMAL", value: 0xDC},
//{name: "KEYPAD HEXADECIMAL", value: 0xDD},
//{name: "LEFT CONTROL", value: 0xE0},
//{name: "LEFT SHIFT", value: 0xE1},
//{name: "LEFT ALT", value: 0xE2},
//{name: "LEFT GUI", value: 0xE3},
//{name: "RIGHT CONTROL", value: 0xE4},
//{name: "RIGHT SHIFT", value: 0xE5},
//{name: "RIGHT ALT", value: 0xE6},
//{name: "RIGHT GUI", value: 0xE7},
//{name: "MEDIA PLAY", value: 0xE8},
//{name: "MEDIA STOP", value: 0xE9},
//{name: "MEDIA PREVIOUS TRACK", value: 0xEA},
//{name: "MEDIA NEXT TRACK", value: 0xEB},
//{name: "MEDIA EJECT", value: 0xEC},
//{name: "MEDIA VOLUME UP", value: 0xED},
//{name: "MEDIA VOLUME DOWN", value: 0xEE},
//{name: "MEDIA MUTE", value: 0xEF},
//{name: "MEDIA WWW", value: 0xF0},
//{name: "MEDIA BACKWARD", value: 0xF1},
//{name: "MEDIA FORWARD", value: 0xF2},
//{name: "MEDIA CANCEL", value: 0xF3},
//{name: "MEDIA SEARCH", value: 0xF4},
//{name: "MEDIA SLEEP", value: 0xF8},
//{name: "MEDIA LOCK", value: 0xF9},
//{name: "MEDIA RELOAD", value: 0xFA},
//{name: "MEDIA CALCULATOR", value: 0xFB}
//{name: "NON_US_HASHMARK_AND_TILDE", value: 0x32},

})(window, document);