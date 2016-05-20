import {Component} from "angular2/core";
import {Temp1} from './temp1';
var client;

@Component({
  selector: 'app',
  directives: [Temp1],
  template: `
  	<div>Hello gorld</div>
  	<temp1 [tempe1]="temp1" [timmr]="tmr1"></temp1><br>
  	`
})
export class App {
	tempe1;
	tmr1;
	constructor(){
		this.temp1 = "mydog";
		this.tmr1 = 14;
	}
}