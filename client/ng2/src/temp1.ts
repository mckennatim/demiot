import {Component, Input} from 'angular2/core';
import {mqtt} from 'mqtt';



@Component({
    selector: 'temp1',
    template: `<h3>fred {{tempe1}} {{timmr}}</h3>
    {{mqtt}}`
})
export class Temp1 {
    @Input() tempe1;
    @Input() timmr;
}