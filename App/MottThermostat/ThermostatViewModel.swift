//
//  ThermostatViewModel.swift
//  MottThermostat
//
//  Created by Charles Berghausen on 6/3/24.
//

import Foundation
import Observation

@Observable
final class ThermostatViewModel: Sendable {
    private var thermostatResponse: ThermostatResponse = .init(
        mode: .onUntilStopped, thermostatSetting: .off, nextUpdateTime: nil
    )

    private let network = Network()

    init() {
        getCurrentACSetting()
    }

    var toggleButtonText: String {
        switch thermostatResponse.thermostatSetting {
        case .on:
            "Turn Air Off"
        case .off:
            "Turn Air On"
        }
    }

    func toggleAC() {
        thermostatResponse.thermostatSetting == .on ? setAcTo(.off) : setAcTo(.on)
    }

    private func getCurrentACSetting() {
        Task {
            do {
                thermostatResponse = try await network.getCurrentACMode()
            }
            catch {
                parseError(error)
            }
        }
    }

    func setAcTo(_ setting: ThermostatSetting) {
        Task {
            do {
                thermostatResponse = try await network.setAC(to: setting)
            }
            catch {
                parseError(error)
            }
        }
    }

    private func parseError(_ error: Error) {
        let nsError = (error as NSError)
        if nsError.code == NSURLErrorTimedOut || nsError.domain == NSURLErrorDomain {
            print("Timeout. The thermostat is not connected to the network.")
        }
        else {
            print(error)
        }
    }
}
