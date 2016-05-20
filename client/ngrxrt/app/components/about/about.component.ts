import {Component} from '@angular/core';

@Component({
    selector: 'about',
    template: `<h3>about {{me}} basic route</h3>`
})

export class AboutComponent {
	public me: string = "ulysses"  
}