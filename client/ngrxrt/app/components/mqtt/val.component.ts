import {Component} from '@angular/core';

@Component({
    selector: 'val',
    template: `
    	<h3>about {{me}} basic route</h3>
			<route-view></route-view>
    `
})

export class ValComponent {
	public me: string = "val component"
}