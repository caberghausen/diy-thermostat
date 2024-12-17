//
//  MottThermostatApp.swift
//  MottThermostat
//
//  Created by Charles Berghausen on 6/3/24.
//

import AppIntents
import SwiftUI

@main
struct MottThermostatApp: App {
    var body: some Scene {
        WindowGroup {
            ThermostatView()
        }
    }
}

struct TurnACOn: AppIntent {
    static var title: LocalizedStringResource = "Turn Air On"

    func perform() async throws -> some IntentResult {
        let viewModel = ThermostatViewModel()
        viewModel.setAcTo(.on)

        return .result()
    }
}

struct TurnACOff: AppIntent {
    static var title: LocalizedStringResource = "Turn Air Off"

    func perform() async throws -> some IntentResult {
        let viewModel = ThermostatViewModel()
        viewModel.setAcTo(.off)

        return .result()
    }
}
