import {bootstrap}    from '@angular/platform-browser-dynamic'
import { provideRouter, Routes } from '@ngrx/router';
import { LocationStrategy,HashLocationStrategy } from '@angular/common';
import { provide } from '@angular/core';
import {provideStore} from '@ngrx/store';

import {AppComponent} from './components/app.component'
import {HelpComponent} from './components/help/help.component'
import {AboutComponent} from './components/about/about.component'
import {CounterComponent} from './components/counter/counter.component.ts'
import {PartyComponent} from './components/party/party.component';


import {counter} from './reducers/counter';
import {people} from './reducers/people';
import {filter} from './reducers/filter';



const routes: Routes = [
	{ path: '/', component: AboutComponent },
	{ path: '/about', component: AboutComponent },
	{ path: '/help', component: HelpComponent },
	{ path: '/cnt', component:CounterComponent},
	{ path: '/party', component:PartyComponent}
]

bootstrap(AppComponent, [
	provideRouter(routes),
	provide(LocationStrategy, { useClass: HashLocationStrategy }),
	provideStore({ counter, people, filter }, { counter: 0})
]);
// bootstrap(AppComponent, [
// 	provideRouter(routes)
// ]);
