import {Component, OnInit, AfterViewInit, OnDestroy} from '@angular/core';
import * as mqtt from 'mqtt';

@Component({
    selector: 'mqtt',
    template: `<h3>about Mqtt {{me}} {{frog}} </h3>
    <button type="button" (click)="subscribe($event)">Subscribe to WebSocket</button>
    `
})

export class MqttComponent implements AfterViewInit, OnDestroy {
	public me: string = "ulysses";
	public frog: string;
	public client: any; 
	public deviceId = 'CYURD001';
	public statu = this.deviceId + '/status';
	public tmr = this.deviceId + '/tmr';
	public progs = this.deviceId + '/progs';
	
	ngAfterViewInit() {
		var self = this;
		this.client = mqtt.connect('ws://10.0.1.100:3333')
		this.client.on('connect', function() {
			console.log('maybe connected');
			self.client.subscribe(self.statu)
			self.client.subscribe(self.tmr)
			self.client.subscribe(self.progs)
			self.client.publish('presence', 'Web Client is alive.. Test Ping! ');
			self.client.on('message', function(topic, payload) {
				console.log(topic)
				var pls = payload.toString()
				console.log(pls)
				var plo = JSON.parse(pls)
				console.log(plo)
				self.frog = plo.temp1;
			})
		});
	}

	ngOnDestroy() {
		console.log('cleint disconnectin')
		this.client.publish('presence', 'Help, wants to close! ');
		this.client.end();
	}

	subscribe($event) {
		console.log("trying to unsubscribe to ws");
		console.log($event);
		this.client.end();
	}

}