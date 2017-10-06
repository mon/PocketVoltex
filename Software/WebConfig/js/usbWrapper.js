// Perhaps at some point I will release an Electron app, too
// This will facilitate that.

(function(window, document, navigator) {
"use strict";

// otherwise race condition
var UsbWrapper_init = function() {

    var UsbWrapper = new function() {
        this.hasUSB = navigator && navigator.usb;
        
        if(!this.hasUSB)
            return;
        
        // returns a promise
        this.connect = (vids, pids) => {
            var filters = [];
            for(let i = 0; i < vids.length; i++) {
                filters.push({ vendorId: vids[i], productId: pids[i] });
            }
            return navigator.usb.requestDevice({filters: filters})
            .catch(error => {
                console.log(error);
                return null;
            });
        };
        
        this.usb = navigator.usb;
    };

    window.UsbWrapper = UsbWrapper;
}

window.UsbWrapper_init = UsbWrapper_init;

})(window, document, navigator);