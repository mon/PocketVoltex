(function(window, document) {
"use strict";

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
    constructor(maxBright) {
        super();
        this.maxBright = maxBright;
    }
    
    read(buffer) {
        var rgb = [];
        for(var i = 0; i < 3; i++)
            rgb[i] = (buffer.read(1) / this.maxBright) * 255;
        this.picker.fromRGB(rgb[0], rgb[1], rgb[2]);
    }
    
    write(buffer) {
        for(var i = 0; i < 3; i++)
            buffer.write((this.picker.rgb[i] / 255) * this.maxBright);
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
        var keys = buffer.read(this.byteCount, true);
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
            var label = null;
            if(setting.labels) {
                label = document.createElement("div");
                label.textContent = setting.labels[i] + ': ';
                label.className = 'keySelection';
                entry.appendChild(label);
            }
            var select = document.createElement("select");
            scancodes.forEach(function(code) {
                var option = document.createElement("option");
                option.value = code.value;
                option.textContent = code.name;
                select.appendChild(option);
            });
            
            select.onchange = this.fireCallback.bind(this);
            if(label) {
                label.appendChild(select);
            } else {
                entry.appendChild(select);
            }
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
    constructor() {
        super();
        this.action = new SettingRadio();
        this.keypress = new SettingKeys(1);
        this.action.setCallback(this.fireCallback.bind(this));
        this.keypress.setCallback(this.fireCallback.bind(this));
    }
    
    createUI(setting) {
        var entry = document.createElement('p');
        entry.textContent = setting.name + ':';
        
        entry.appendChild(this.action.createUI({
            id: setting.id + '-action', name: 'Action',
            options : [{name: 'Keypress', val: 0},
                       {name: 'Macro PIN', val: 1},
                       {name: 'Cycle LED pattern', val: 2},
                       {name: 'Toggle LEDs', val: 3}]
        }));
        entry.appendChild(this.keypress.createUI({name: 'Key'}));
        
        return entry;
    }
    
    read(buffer) {
        this.action.read(buffer);
        this.keypress.read(buffer);
    }
    
    write(buffer) {
        this.action.write(buffer);
        this.keypress.write(buffer);
    }
}

var scancodes = [
  {name: "None", value: 0x00},
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

window.SettingBool = SettingBool;
window.SettingRGB = SettingRGB;
window.SettingRadio = SettingRadio;
window.SettingKeys = SettingKeys;
window.SettingPin = SettingPin;
window.SettingMacro = SettingMacro;

})(window, document);