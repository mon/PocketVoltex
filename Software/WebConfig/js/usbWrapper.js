(function(window, document, navigator) {
"use strict";

var UsbWrapper = new function() {
    this.hasUSB = navigator && navigator.usb;
    
    // returns a promise
    this.connect = (vid, pid) => {
        return navigator.usb.requestDevice({ filters: [
            { vendorId: vid, productId: pid }] })
        .catch(error => {
            console.log(error);
            return null;
        });
    };
};

window.UsbWrapper = UsbWrapper;
})(window, document, navigator);