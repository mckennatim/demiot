import {Component} from '@angular/core';
import { RouteParams } from '@ngrx/router';
import { Observable } from 'rxjs/Observable';

@Component({
    selector: 'aval',
    template: `<h3>{{ id$ | async }} dog</h3>
    <route-view></route-view>`
})

export class AvalComponent {
	public me: string = "aval component";
	id$: Observable<any>; 
	
	constructor(routeParams$: RouteParams) {
		this.id$ = routeParams$.pluck('id');
	}
}