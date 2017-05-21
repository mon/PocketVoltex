(function(window, document) {
"use strict";

var vid = 0x16D0;
var pid = 0x0A6D;
var UsbWrapper = window.UsbWrapper;

var device;

var SWITCH_COUNT = 8;
var BRIGHTNESS_MAX = 127;
var CONFIG_OUT_EPADDR = 1;
var CONFIG_IN_EPADDR  = 2;
var packetSize = 64;

var commands = {
    GETCONFIG : 1,
    SETCONFIG : 2,
    VERSION   : 3,
    RESET :     42
};

var configDisplay = [
    {id: 'joystickMode', name: 'Input mode',
        options : [{name: 'Keyboard/Mouse', val: 0}, {name: 'Joystick', val: 1}]},
    {id: 'switches', name: 'Keyboard bindings'},
    {id: 'macroClick', name: 'Macro click'},
    {id: 'macroHold', name: 'Macro longpress'},
    {id: 'macroPin', name: 'Macro PIN'},
    {id: 'lightsOn', name: 'Enable LEDs', children: [
        {id: 'hidLights', name: 'HID Lights'},
        {id: 'keyLights', name: 'Key lights', children: [        
            {id: 'btColour', name: 'BT colour'},
            {id: 'fxColour', name: 'FX colour'},
        ]},
        {id: 'knobLights', name: 'Knob lights', children: [
            {id: 'knobL', name: 'VOL-L colour'},
            {id: 'knobR', name: 'VOL-R colour'},
        ]},
        {id: 'lightPattern', name: 'Lights pattern',
            options : [{name: 'None',     val : 1},
                       {name: 'Solid',    val: 2},
                       {name: 'Breathe',  val: 4},
                       {name: 'Follower', val: 3}]},
        {id: 'breatheColour', name: 'Solid/Breathe colour'},
    ]},
];

// to match with firmware
var settingOrder = [
    'switches',
    'btColour',
    'fxColour',
    'breatheColour',
    'knobL',
    'knobR',
    'lightsOn',
    'hidLights',
    'keyLights',
    'knobLights',
    'lightPattern',
    'macroClick',
    'macroHold',
    'macroPin',
    'joystickMode'
];

var defaultConfig = {
    switches   : [],
    ledsOn     : true,
    //macroClick : KP_PLUS,
    macroPin   : [0,0,0,0],
}

var visibleLog = function(html) {
    console.log(html);
    document.getElementById('logview').innerHTML += html + '<br/>';
}

class BinaryBuffer {
    constructor(view) {
        this.view = view;
        this.offset = 0;
    }
    
    read(size) {
        let ret;
        if(size == 1) {
            ret = this.view.getUint8(this.offset);
        } else {
            ret = [];
            for(var i = 0; i < size; i++) {
                ret[i] = this.view.getUint8(this.offset+i);
            }
        }
        this.offset += size;
        return ret;
    }
    
    write(data) {
        if(Array.isArray(data)) {
            for(let i = 0; i < data.length; i++) {
                this.view[this.offset+i] = data[i];
            }
            this.offset += data.length;
        } else {
            this.view[this.offset++] = data;
        }
    }
    
    rewind() {
        this.offset = 0;
    }
}

class Setting {
    setCallback(callback) {
        this.callback = callback;
    }
    
    fireCallback() {
        if(this.callback)
            this.callback(this);
    }
}

// All settings must have createUI, read and write.
// When the UI changes, call fireCallback to update the device config
class SettingBool extends Setting {
    read(buffer) {
        this.box.checked = !!buffer.read(1);
    }
    
    write(buffer) {
        buffer.write(this.box.checked ? 1 : 0);
    }
    
    createUI(setting) {
        var container = document.createElement('div');
        var check = document.createElement('input');
        check.type = 'checkbox';
        check.id = 'check-' + setting.id;
        check.onclick = this.fireCallback.bind(this);
        this.box = check;
        
        var label = document.createElement('label');
        label.htmlFor = check.id;
        label.textContent = setting.name;
        
        container.appendChild(check);
        container.appendChild(label);
        return container;
    }
}

class SettingRGB extends Setting {
    read(buffer) {
        var rgb = [];
        for(var i = 0; i < 3; i++)
            rgb[i] = (buffer.read(1) / BRIGHTNESS_MAX) * 255;
        this.picker.fromRGB(rgb[0], rgb[1], rgb[2]);
    }
    
    write(buffer) {
        for(var i = 0; i < 3; i++)
            buffer.write((this.picker.rgb[i] / 255) * BRIGHTNESS_MAX);
    }
    
    createUI(setting) {
        var label = document.createElement('p');
        label.textContent = setting.name + ':';
        
        var colourPicker = document.createElement('input');
        colourPicker.className = 'colourPicker jscolor {valueElement:null}';
        colourPicker.onchange = this.fireCallback.bind(this);
        this.picker = new jscolor(colourPicker)
        
        label.appendChild(colourPicker);
        return label;
    }
}

class SettingRadio extends Setting {
    read(buffer) {
        var val = buffer.read(1);
        
        for(var i = 0; i < this.options.length; i++) {
            let opt = this.options[i];
            if(opt.val == val) {
                opt.radio.checked = true;
                return;
            }
        }
        // sane default
        this.options[0].radio.checked = true;
    }
    
    write(buffer) {
        for(var i = 0; i < this.options.length; i++) {
            let opt = this.options[i];
            if(opt.radio.checked) {
                buffer.write(opt.val);
                return;
            }
        }
        // sane default
        buffer.write(this.options[0].val);
    }
    
    createUI(setting) {
        this.options = setting.options;
        var container = document.createElement('div');
        container.textContent = setting.name + ':';
        
        var radioDiv = document.createElement('div');
        radioDiv.className = 'nestedSetting';

        for(var i = 0; i < setting.options.length; i++) {
            var opt = setting.options[i];
            
            var radio = document.createElement('input');
            radio.type = 'radio';
            radio.id = setting.id + '-' + i;
            radio.name = setting.id;
            radio.onclick = this.fireCallback.bind(this);
            opt.radio = radio;
            
            var label = document.createElement('label');
            label.htmlFor = radio.id;
            label.textContent = opt.name;
            
            var radioContainer = document.createElement('p');
            radioContainer.appendChild(radio);
            radioContainer.appendChild(label);
            radioDiv.appendChild(radioContainer);
        }
        
        container.appendChild(radioDiv);
        return container;
    }
}

class SettingKeys extends Setting {
    constructor(switchCount, ignoreLast = false) {
        super();
        this.byteCount = switchCount;
        // hacks upon hacks, this stuff
        this.ignoreLast = ignoreLast;
        this.switchCount = ignoreLast ? switchCount - 1 : switchCount;
    }
    
    read(buffer) {
        var keys = buffer.read(this.byteCount);
        for(var i = 0; i < this.switchCount; i++) {
            this.selects[i].value = keys[i];
        }
    }
    
    write(buffer) {
        for(var i = 0; i < this.selects.length; i++) {
            buffer.write(this.selects[i].value);
        }
        if(this.ignoreLast) {
            buffer.write(0);
        }
    }
    
    createUI(setting) {
        var entry = document.createElement('p');
        entry.textContent = setting.name + ':';
        
        this.selects = [];
        for(var i = 0; i < this.switchCount; i++) {
            var select = document.createElement("select");
            scancodes.forEach(function(code) {
                var option = document.createElement("option");
                option.value = code.value;
                option.textContent = code.name;
                select.appendChild(option);
            });
            
            select.onchange = this.fireCallback.bind(this);
            entry.appendChild(select);
            this.selects.push(select);
        }
        return entry;
    }
}

class SettingPin extends Setting {
    createUI(setting) {
        var entry = document.createElement('p');
        entry.textContent = setting.name + ':';
        
        var input = document.createElement('input');
        input.type = 'password';
        input.className = 'pinEntry';
        input.textContent = '0000';
        
        entry.appendChild(input);
        return entry;
    }
}

class SettingMacro extends Setting {
    createUI(setting) {
        var entry = document.createElement('p');
        entry.textContent = setting.name + ':';
        
        return entry;
    }
    
    read(buffer) {
        this.keys = buffer.read(2);
    }
    
    write(buffer) {
        buffer.write(this.keys);
    }
}

class ConfigValues {
    constructor() {
        this.switches      = new SettingKeys(SWITCH_COUNT, true);
        this.btColour      = new SettingRGB();
        this.fxColour      = new SettingRGB();
        this.breatheColour = new SettingRGB();
        this.knobL         = new SettingRGB();
        this.knobR         = new SettingRGB();
        this.lightsOn      = new SettingBool();
        this.hidLights     = new SettingBool();
        this.keyLights     = new SettingBool();
        this.knobLights    = new SettingBool();
        this.lightPattern  = new SettingRadio();
        this.macroClick    = new SettingMacro();
        this.macroHold     = new SettingMacro();
        this.macroPin      = new SettingKeys(4);
        this.joystickMode  = new SettingRadio();
    }
    
    read(view, writeCallback) {
        var buffer = new BinaryBuffer(view);
        // skip the returned command byte
        buffer.read(1);
        
        settingOrder.forEach(setting => {
            this[setting].read(buffer);
            this[setting].setCallback(writeCallback);
        });
    }

    write(view) {
        var buffer = new BinaryBuffer(view);
        buffer.write(commands.SETCONFIG);
        
        settingOrder.forEach(setting => {
            this[setting].write(buffer);
        });
    }

    getArray(view, offset, size) {
        var ret = [];
        for(var i = 0; i < size; i++) {
            ret[i] = view.getUint8(offset+i);
        }
        return ret;
    };

    getColour(view, offset) {
        return 'rgb(' + this.getArray(view, offset, 3).join() + ')';
    };

    getKeys(view, offset, size) {
        var keys = [];
        for(let i = 0; i < size; i++) {
            keys[i] = this.getKey(view, offset++);
        }
        return keys;
    }

    getKey(view, offset) {
        var key = view.getUint8(offset);
        return scancodes.find(code => {return code.value == key});
    }
}

class Config {
    constructor() {
        this.optionsDiv = document.getElementById('configOptions');
        this.config = new ConfigValues();
        // DEBUG
        //this.enableUI();
    }
    
    connect() {
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
            if(device.productName == 'Voltex Boot') {
                return this.loadFirmware();
            } else {
                return this.loadConfig();
            }
        })
        .catch(error => {
            //alert(error);
            visibleLog(error);
            if(device && device.opened)
                device.close();
        });
    }
    
    loadFirmware() {
        visibleLog("Downloading fimware...");
        return getLatest()
        .then(info => {
            return downloadLatest(info.version);
        })
        .then(firmware => {
            visibleLog("Flashing firmware...");
            return programLatest(device, firmware);
        })
        .then(() => {
            visibleLog("Done!");
        })
    }
    
    loadConfig() {
        console.log("Selected, claiming interface...");
        return device.claimInterface(0)
        .then(() => {
            visibleLog("Opened!");
            return this.readVersion();
        })
        .then(version => {
            console.log("Found Pocket Voltex v" + version/10.0);
            this.enableUI();
            return this.readConfig();
        })
    }
    
    readVersion() {
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
    }
    
    readConfig() {
        if(!device || !device.opened)
            return Promise.reject("Device not opened");
        var data = new Uint8Array(packetSize);
        data[0] = commands.GETCONFIG;
        return device.transferOut(CONFIG_OUT_EPADDR, data)
        .then(() => device.transferIn(CONFIG_IN_EPADDR, packetSize))
        .then(result => {
            console.log("Got config response of len", result.data.buffer.byteLength);
            this.config.read(result.data, this.writeConfig.bind(this));
        });
    }
    
    writeConfig() {
        if(!device || !device.opened)
            return Promise.reject("Device not opened");
        
        console.log("Writing config");
        var data = new Uint8Array(packetSize);
        this.config.write(data);
        return device.transferOut(CONFIG_OUT_EPADDR, data);
    }
    
    close() {
        if(!device || !device.opened)
            return Promise.reject("Device not opened");
        visibleLog("Closing device...");
        return device.close();
    }
    
    rebootToBootloader() {
        if(!device || !device.opened)
            return Promise.reject("Device not opened");
        var data = new Uint8Array(packetSize);
        data[0] = commands.RESET;
        return device.transferOut(CONFIG_OUT_EPADDR, data);
    };
    
    populateSettings(parent, settings) {
        settings.forEach(setting => {
            var settingObj = this.config[setting.id];
            var container = settingObj.createUI(setting);
            settingObj.setCallback(this.writeConfig.bind(this));
            
            if(setting.children) {
                var newParent = document.createElement('div');
                newParent.className = 'nestedSetting';
                
                this.populateSettings(newParent, setting.children);
                container.appendChild(newParent);
            }
            parent.appendChild(container);
        });
    }
    
    enableUI() {
        //document.getElementById('configBox').classList.remove('hidden', 'disabled');
        document.getElementById('connect').classList.add('hidden');
        this.populateSettings(this.optionsDiv, configDisplay);
    }
};

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

})(window, document);